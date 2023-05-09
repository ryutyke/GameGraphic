#include "Cube/BaseCube.h"

BaseCube::BaseCube(_In_ const std::filesystem::path& textureFilePath)
    : Renderable(textureFilePath)
{
}

BaseCube::BaseCube(_In_ const XMFLOAT4& outputColor)
    : Renderable(outputColor)
{
}

HRESULT BaseCube::Initialize(_In_ ID3D11Device* pDevice, _In_ ID3D11DeviceContext* pImmediateContext)
{
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
