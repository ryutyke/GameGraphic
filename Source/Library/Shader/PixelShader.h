#pragma once

#include "Common.h"

#include "Shader/Shader.h"

class PixelShader : public Shader
{
public:
	PixelShader(_In_ PCWSTR pszFileName, _In_ PCSTR pszEntryPoint, _In_ PCSTR  pszShaderModel);
	virtual ~PixelShader() = default;

	virtual HRESULT Initialize(_In_ ID3D11Device* pDevice) override;

	ComPtr<ID3D11PixelShader>& GetPixelShader();

protected:
	ComPtr<ID3D11PixelShader> m_pixelShader;
};