#include "Renderer/Renderer.h"

Renderer::Renderer()
    : m_driverType(D3D_DRIVER_TYPE_NULL)
    , m_featureLevel(D3D_FEATURE_LEVEL_11_0)
    , m_d3dDevice()
    , m_d3dDevice1()
    , m_immediateContext()
    , m_immediateContext1()
    , m_swapChain()
    , m_swapChain1()
    , m_renderTargetView()
    , m_depthStencil()
    , m_depthStencilView()
    , m_cbChangeOnResize()
    , m_camera(XMVectorSet(0.0f, 12.0f, -20.0f, 0.0f))
    , m_projection()
    , m_renderables()
    , m_models()
    , m_aPointLights()
    , m_vertexShaders()
    , m_pixelShaders()
{
}

HRESULT Renderer::InitDevice(_In_ HWND hWnd)
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect(hWnd, &rc);
    UINT width = static_cast<UINT>(rc.right - rc.left);
    UINT height = static_cast<UINT>(rc.bottom - rc.top);

    UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        m_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDevice(nullptr, m_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
            D3D11_SDK_VERSION, m_d3dDevice.GetAddressOf(), &m_featureLevel, m_immediateContext.GetAddressOf());

        if (hr == E_INVALIDARG)
        {
            // Direct X 11.0에서는 D3D_FEATURE_LEVEL_11_1을 인식하지 못할 수 있음. 이 경우 다시 만들어줌.

            hr = D3D11CreateDevice(nullptr, m_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
                D3D11_SDK_VERSION, m_d3dDevice.GetAddressOf(), &m_featureLevel, m_immediateContext.GetAddressOf());
        }

        if (SUCCEEDED(hr))
            break;
    }
    if (FAILED(hr))
        return hr;

    // DXGI Device를 통해 DXGI Factory를 받아옴.
    ComPtr<IDXGIFactory1> dxgiFactory;
    ComPtr<IDXGIDevice> dxgiDevice;

    hr = m_d3dDevice.As(&dxgiDevice);
    if (SUCCEEDED(hr))
    {
        ComPtr<IDXGIAdapter> adapter;
        hr = dxgiDevice->GetAdapter(&adapter);
        if (SUCCEEDED(hr))
        {
            hr = adapter->GetParent(IID_PPV_ARGS(&dxgiFactory));
        }
    }

    if (FAILED(hr))
        return hr;

    ComPtr<IDXGIFactory2> dxgiFactory2;
    hr = dxgiFactory.As(&dxgiFactory2);
    if (SUCCEEDED(hr))
    {
        // DirectX 11.1 or later
        hr = m_d3dDevice.As(&m_d3dDevice1);
        if (SUCCEEDED(hr))
        {
            m_immediateContext.As(&m_immediateContext1);
        }

        DXGI_SWAP_CHAIN_DESC1 sd = {};
        sd.Width = width;
        sd.Height = height;
        sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1;

        hr = dxgiFactory2->CreateSwapChainForHwnd(m_d3dDevice.Get(), hWnd, &sd, nullptr, nullptr, m_swapChain1.GetAddressOf());
        if (SUCCEEDED(hr))
        {
            hr = m_swapChain1.As(&m_swapChain);
        }
    }
    else
    {
        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferCount = 1;
        sd.BufferDesc.Width = width;
        sd.BufferDesc.Height = height;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hWnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;

        hr = dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &sd, m_swapChain.GetAddressOf());
    }

    if (FAILED(hr))
        return hr;

    ComPtr<ID3D11Texture2D> pBackBuffer;
    hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (FAILED(hr))
        return hr;

    hr = m_d3dDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, m_renderTargetView.GetAddressOf());
    if (FAILED(hr))
        return hr;

    D3D11_TEXTURE2D_DESC descDepth = {};
    {
        descDepth.Width = width;
        descDepth.Height = height;
        descDepth.MipLevels = 1;
        descDepth.ArraySize = 1;
        descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        descDepth.SampleDesc.Count = 1;
        descDepth.SampleDesc.Quality = 0;
        descDepth.Usage = D3D11_USAGE_DEFAULT;
        descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        descDepth.CPUAccessFlags = 0;
        descDepth.MiscFlags = 0;
    }

    hr = m_d3dDevice->CreateTexture2D(&descDepth, nullptr, m_depthStencil.GetAddressOf());

    if (FAILED(hr))
        return hr;

    // Depth Stencil buffer 바인딩
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;

    hr = m_d3dDevice->CreateDepthStencilView(m_depthStencil.Get(), &descDSV, m_depthStencilView.GetAddressOf());
    if (FAILED(hr))
        return hr;

    m_immediateContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());


    // Setup the viewport
    D3D11_VIEWPORT vp;
    {
        vp.Width = static_cast<FLOAT>(width);
        vp.Height = static_cast<FLOAT>(height);
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
    }
    m_immediateContext->RSSetViewports(1, &vp);

    m_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D11_BUFFER_DESC bd =
    {
        .ByteWidth = sizeof(CBChangeOnResize),
        .Usage = D3D11_USAGE_DEFAULT,
        .BindFlags = D3D11_BIND_CONSTANT_BUFFER,
        .CPUAccessFlags = 0
    };
    hr = m_d3dDevice->CreateBuffer(&bd, nullptr, m_cbChangeOnResize.GetAddressOf());
    if (FAILED(hr))
        return hr;

    m_projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, static_cast<FLOAT>(width) / static_cast<FLOAT>(height), 0.01f, 100.0f);

    CBChangeOnResize cbChangesOnResize =
    {
        .Projection = XMMatrixTranspose(m_projection)
    };
    m_immediateContext->UpdateSubresource(m_cbChangeOnResize.Get(), 0, nullptr, &cbChangesOnResize, 0, 0);

    bd.ByteWidth = sizeof(CBLights);
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0u;

    hr = m_d3dDevice->CreateBuffer(&bd, nullptr, m_cbLights.GetAddressOf());
    if (FAILED(hr))
        return hr;

    CBLights cbLights = {};
    for (size_t i = 0; i < NUM_LIGHTS; ++i)
    {
        cbLights.LightPositions[i] = m_aPointLights[i]->GetPosition();
        cbLights.LightColors[i] = m_aPointLights[i]->GetColor();
    }
    m_immediateContext->UpdateSubresource(m_cbLights.Get(), 0, nullptr, &cbLights, 0, 0);

    m_camera.Initialize(m_d3dDevice.Get(), m_immediateContext.Get());

    for (auto it = m_vertexShaders.begin(); it != m_vertexShaders.end(); ++it)
    {
        it->second->Initialize(m_d3dDevice.Get());
    }

    for (auto it = m_pixelShaders.begin(); it != m_pixelShaders.end(); ++it)
    {
        it->second->Initialize(m_d3dDevice.Get());
    }

    for (auto it = m_renderables.begin(); it != m_renderables.end(); ++it)
    {
        it->second->Initialize(m_d3dDevice.Get(), m_immediateContext.Get());
    }

    for (auto it = m_models.begin(); it != m_models.end(); ++it)
    {
        it->second->Initialize(m_d3dDevice.Get(), m_immediateContext.Get());
    }

    return S_OK;
}

HRESULT Renderer::AddRenderable(_In_ PCWSTR pszRenderableName, _In_ const std::shared_ptr<Renderable>& renderable)
{
    if (m_renderables.contains(pszRenderableName))
    {
        return E_FAIL;
    }

    m_renderables[pszRenderableName] = renderable;

    return S_OK;
}

HRESULT Renderer::AddModel(_In_ PCWSTR pszModelName, _In_ const std::shared_ptr<Model>& pModel)
{
    if (m_models.contains(pszModelName))
    {
        return E_FAIL;
    }

    m_models[pszModelName] = pModel;

    return S_OK;
}

HRESULT Renderer::AddPointLight(_In_ size_t index, _In_ const std::shared_ptr<PointLight>& pPointLight)
{
    HRESULT hr = S_OK;

    if (index >= NUM_LIGHTS)
    {
        return E_FAIL;
    }

    m_aPointLights[index] = pPointLight;

    return hr;
}

HRESULT Renderer::AddVertexShader(_In_ PCWSTR pszVertexShaderName, _In_ const std::shared_ptr<VertexShader>& vertexShader)
{
    if (m_vertexShaders.contains(pszVertexShaderName))
    {
        return E_FAIL;
    }

    m_vertexShaders[pszVertexShaderName] = vertexShader;

    return S_OK;
}

HRESULT Renderer::AddPixelShader(_In_ PCWSTR pszPixelShaderName, _In_ const std::shared_ptr<PixelShader>& pixelShader)
{
    if (m_pixelShaders.contains(pszPixelShaderName))
    {
        return E_FAIL;
    }

    m_pixelShaders[pszPixelShaderName] = pixelShader;

    return S_OK;
}

void Renderer::HandleInput(_In_ const InputDirections& directions, _In_ FLOAT deltaTime)
{
    m_camera.HandleInput(directions, deltaTime);
}

void Renderer::Update(_In_ FLOAT deltaTime)
{
    for (auto it = m_renderables.begin(); it != m_renderables.end(); ++it)
    {
        it->second->Update(deltaTime);
    }

    for (auto it = m_models.begin(); it != m_models.end(); ++it)
    {
        it->second->Update(deltaTime);
    }

    for (UINT i = 0; i < NUM_LIGHTS; ++i)
    {
        m_aPointLights[i]->Update(deltaTime);
    }

    m_camera.Update(deltaTime);
}

void Renderer::Render()
{
    // Clear the back buffer
    m_immediateContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::MidnightBlue);

    // Clear the depth buffer to 1.0 (max depth)
    m_immediateContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

    CBChangeOnCameraMovement cbChangeOnCameraMovement =
    {
        .View = XMMatrixTranspose(m_camera.GetView())
    };
    XMStoreFloat4(&cbChangeOnCameraMovement.CameraPosition, m_camera.GetEye());
    m_immediateContext->UpdateSubresource(m_camera.GetConstantBuffer().Get(), 0, nullptr, &cbChangeOnCameraMovement, 0, 0);

    CBLights cbLights = {};
    for (size_t i = 0; i < NUM_LIGHTS; ++i)
    {
        cbLights.LightPositions[i] = m_aPointLights[i]->GetPosition();
        cbLights.LightColors[i] = m_aPointLights[i]->GetColor();
    }
    m_immediateContext->UpdateSubresource(m_cbLights.Get(), 0, nullptr, &cbLights, 0, 0);

    for (auto it = m_renderables.begin(); it != m_renderables.end(); ++it)
    {
        // Set vertex buffer
        UINT uStride = sizeof(SimpleVertex);
        UINT uOffset = 0;
        m_immediateContext->IASetVertexBuffers(0u, 1u, it->second->GetVertexBuffer().GetAddressOf(), &uStride, &uOffset);

        // Set index buffer
        m_immediateContext->IASetIndexBuffer(it->second->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

        // Set the input layout
        m_immediateContext->IASetInputLayout(it->second->GetVertexLayout().Get());

        // Update variables
        CBChangeEveryFrame cb =
        {
            .World = XMMatrixTranspose(it->second->GetWorldMatrix()),
            .OutputColor = it->second->GetOutputColor()
        };
        m_immediateContext->UpdateSubresource(it->second->GetConstantBuffer().Get(), 0u, nullptr, &cb, 0u, 0u);

        // Render a triangle
        m_immediateContext->VSSetShader(it->second->GetVertexShader().Get(), nullptr, 0u);
        m_immediateContext->VSSetConstantBuffers(0, 1, m_camera.GetConstantBuffer().GetAddressOf());
        m_immediateContext->VSSetConstantBuffers(1, 1, m_cbChangeOnResize.GetAddressOf());
        m_immediateContext->VSSetConstantBuffers(2, 1, it->second->GetConstantBuffer().GetAddressOf());
        m_immediateContext->PSSetShader(it->second->GetPixelShader().Get(), nullptr, 0u);
        m_immediateContext->PSSetConstantBuffers(0, 1, m_camera.GetConstantBuffer().GetAddressOf());
        m_immediateContext->PSSetConstantBuffers(2, 1, it->second->GetConstantBuffer().GetAddressOf());
        m_immediateContext->PSSetConstantBuffers(3, 1, m_cbLights.GetAddressOf());

        for (UINT i = 0u; i < it->second->GetNumMeshes(); ++i)
        {
            UINT uMaterialIndex = it->second->GetMesh(i).uMaterialIndex;

            assert(uMaterialIndex < it->second->GetNumMaterials());

            // Render a triangle
            if (it->second->GetMaterial(uMaterialIndex).pDiffuse)
            {
                m_immediateContext->PSSetShaderResources(
                    0u,
                    1u,
                    it->second->GetMaterial(uMaterialIndex).pDiffuse->GetTextureResourceView().GetAddressOf()
                );
                m_immediateContext->PSSetSamplers(
                    0u,
                    1u,
                    it->second->GetMaterial(uMaterialIndex).pDiffuse->GetSamplerState().GetAddressOf()
                );
            }


            m_immediateContext->DrawIndexed(
                it->second->GetMesh(i).uNumIndices,
                it->second->GetMesh(i).uBaseIndex,
                static_cast<INT>(it->second->GetMesh(i).uBaseVertex)
            );
        }
    }

    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
    TODO: animation render 구현.
    -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
    for (auto it = m_models.begin(); it != m_models.end(); ++it)
    {
        // Set vertex buffer
        UINT aStrides[2] = { static_cast<UINT>(sizeof(SimpleVertex)), static_cast<UINT>(sizeof(AnimationData)) };
        UINT aOffsets[2] = { 0u, 0u };
        ID3D11Buffer* aBuffers[2]
        {
            it->second->GetVertexBuffer().Get(),
            it->second->GetAnimationBuffer().Get()
        };

        m_immediateContext->IASetVertexBuffers(0u, 2u, aBuffers, aStrides, aOffsets);

        // Set index buffer
        m_immediateContext->IASetIndexBuffer(it->second->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

        // Set the input layout
        m_immediateContext->IASetInputLayout(it->second->GetVertexLayout().Get());

        // Update variables
        CBChangeEveryFrame cb =
        {
            .World = XMMatrixTranspose(it->second->GetWorldMatrix()),
            .OutputColor = it->second->GetOutputColor(),
        };
        m_immediateContext->UpdateSubresource(it->second->GetConstantBuffer().Get(), 0u, nullptr, &cb, 0u, 0u);

        CBSkinning* cbSkinning = reinterpret_cast<CBSkinning*>(malloc(sizeof(CBSkinning)));
        for (UINT i = 0u; i < it->second->GetBoneTransforms().size(); ++i)
        {
            cbSkinning->BoneTransforms[i] = XMMatrixTranspose(it->second->GetBoneTransforms().at(i));
        }
        m_immediateContext->UpdateSubresource(it->second->GetSkinningConstantBuffer().Get(), 0u, nullptr, cbSkinning, 0u, 0u);

        m_immediateContext->VSSetShader(it->second->GetVertexShader().Get(), nullptr, 0u);
        m_immediateContext->VSSetConstantBuffers(0, 1, m_camera.GetConstantBuffer().GetAddressOf());
        m_immediateContext->VSSetConstantBuffers(1, 1, m_cbChangeOnResize.GetAddressOf());
        m_immediateContext->VSSetConstantBuffers(2, 1, it->second->GetConstantBuffer().GetAddressOf());
        m_immediateContext->VSSetConstantBuffers(4, 1, it->second->GetSkinningConstantBuffer().GetAddressOf());
        m_immediateContext->PSSetShader(it->second->GetPixelShader().Get(), nullptr, 0u);
        m_immediateContext->PSSetConstantBuffers(0, 1, m_camera.GetConstantBuffer().GetAddressOf());
        m_immediateContext->PSSetConstantBuffers(2, 1, it->second->GetConstantBuffer().GetAddressOf());
        m_immediateContext->PSSetConstantBuffers(3, 1, m_cbLights.GetAddressOf());

        for (UINT i = 0u; i < it->second->GetNumMeshes(); ++i)
        {
            UINT uMaterialIndex = it->second->GetMesh(i).uMaterialIndex;

            assert(uMaterialIndex < it->second->GetNumMaterials());

            // Render a triangle
            if (it->second->GetMaterial(uMaterialIndex).pDiffuse)
            {
                m_immediateContext->PSSetShaderResources(
                    0u,
                    1u,
                    it->second->GetMaterial(uMaterialIndex).pDiffuse->GetTextureResourceView().GetAddressOf()
                );
                m_immediateContext->PSSetSamplers(
                    0u,
                    1u,
                    it->second->GetMaterial(uMaterialIndex).pDiffuse->GetSamplerState().GetAddressOf()
                );
            }

            m_immediateContext->DrawIndexed(
                it->second->GetMesh(i).uNumIndices,
                it->second->GetMesh(i).uBaseIndex,
                static_cast<INT>(it->second->GetMesh(i).uBaseVertex)
            );
        }
    }

    m_swapChain->Present(0, 0);
}

HRESULT Renderer::SetVertexShaderOfRenderable(_In_ PCWSTR pszRenderableName, _In_ PCWSTR pszVertexShaderName)
{
    if (!m_renderables.contains(pszRenderableName) || !m_vertexShaders.contains(pszVertexShaderName))
    {
        return E_FAIL;
    }

    m_renderables[pszRenderableName]->SetVertexShader(m_vertexShaders[pszVertexShaderName]);

    return S_OK;
}

HRESULT Renderer::SetPixelShaderOfRenderable(_In_ PCWSTR pszRenderableName, _In_ PCWSTR pszPixelShaderName)
{
    if (!m_renderables.contains(pszRenderableName) || !m_pixelShaders.contains(pszPixelShaderName))
    {
        return E_FAIL;
    }

    m_renderables[pszRenderableName]->SetPixelShader(m_pixelShaders[pszPixelShaderName]);

    return S_OK;
}

HRESULT Renderer::SetVertexShaderOfModel(_In_ PCWSTR pszModelName, _In_ PCWSTR pszVertexShaderName)
{
    if (!m_models.contains(pszModelName) || !m_vertexShaders.contains(pszVertexShaderName))
    {
        return E_FAIL;
    }

    m_models[pszModelName]->SetVertexShader(m_vertexShaders[pszVertexShaderName]);

    return S_OK;
}

HRESULT Renderer::SetPixelShaderOfModel(_In_ PCWSTR pszModelName, _In_ PCWSTR pszPixelShaderName)
{
    if (!m_models.contains(pszModelName) || !m_pixelShaders.contains(pszPixelShaderName))
    {
        return E_FAIL;
    }

    m_models[pszModelName]->SetPixelShader(m_pixelShaders[pszPixelShaderName]);

    return S_OK;
}

D3D_DRIVER_TYPE Renderer::GetDriverType() const
{
    return m_driverType;
}