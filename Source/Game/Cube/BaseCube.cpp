#include "Cube/BaseCube.h"

BaseCube::BaseCube(_In_ const XMFLOAT4& outputColor)
    : Renderable(outputColor)
{
}

HRESULT BaseCube::Initialize(_In_ ID3D11Device* pDevice, _In_ ID3D11DeviceContext* pImmediateContext)
{
    BasicMeshEntry basicMeshEntry;
    basicMeshEntry.uNumIndices = NUM_INDICES;
    basicMeshEntry.uMaterialIndex = 0u;

    m_aMeshes.push_back(basicMeshEntry);

    if (HasTexture() > 0)
    {
        HRESULT hr = SetMaterialOfMesh(0, 0);
        if (FAILED(hr))
            return hr;
    }

    return initialize(pDevice, pImmediateContext);
}

const SimpleVertex* BaseCube::getVertices() const
{
    return VERTICES;
}

UINT BaseCube::GetNumVertices() const
{
    return NUM_VERTICES;
}

const WORD* BaseCube::getIndices() const
{
    return INDICES;
}

UINT BaseCube::GetNumIndices() const
{
    return NUM_INDICES;
}
