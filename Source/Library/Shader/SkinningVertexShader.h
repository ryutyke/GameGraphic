#pragma once

#include "Common.h"

#include "Shader/VertexShader.h"

class SkinningVertexShader : public VertexShader
{
public:
    SkinningVertexShader() = delete;
    SkinningVertexShader(_In_ PCWSTR pszFileName, _In_ PCSTR pszEntryPoint, _In_ PCSTR pszShaderModel);
    virtual ~SkinningVertexShader() = default;

    virtual HRESULT Initialize(_In_ ID3D11Device* pDevice) override;
};