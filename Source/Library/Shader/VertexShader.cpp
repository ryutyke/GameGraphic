#include "Shader/VertexShader.h"

VertexShader::VertexShader(_In_ PCWSTR pszFileName, _In_ PCSTR pszEntryPoint, _In_ PCSTR  pszShaderModel)
    :Shader(pszFileName, pszEntryPoint, pszShaderModel)
    , m_vertexShader()
{
}

HRESULT VertexShader::Initialize(_In_ ID3D11Device* pDevice)
{
    ComPtr<ID3DBlob> vsBlob;

    HRESULT hr = compile(vsBlob.GetAddressOf());

    if (FAILED(hr))
    {
        MessageBox(
            nullptr,
            L"The FX file cannot be compiled. Please run this executable from the directory that contains the FX file.",
            L"Error",
            MB_OK
        );
        return hr;
    }

    hr = pDevice->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf());

    if (FAILED(hr))
        return hr;

    D3D11_INPUT_ELEMENT_DESC layouts[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    UINT uNumElements = ARRAYSIZE(layouts);

    hr = pDevice->CreateInputLayout(
        layouts,
        uNumElements,
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        m_vertexLayout.GetAddressOf()
    );

    return hr;
}

ComPtr<ID3D11VertexShader>& VertexShader::GetVertexShader()
{
    return m_vertexShader;
}

ComPtr<ID3D11InputLayout>& VertexShader::GetVertexLayout()
{
    return m_vertexLayout;
}