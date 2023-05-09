#pragma once

#include "Common.h"

#include "Shader/Shader.h"

class VertexShader : public Shader
{
public:
	VertexShader(_In_ PCWSTR pszFileName, _In_ PCSTR pszEntryPoint, _In_ PCSTR  pszShaderModel);
	virtual ~VertexShader() = default;

	virtual HRESULT Initialize(_In_ ID3D11Device* pDevice) override;

	ComPtr<ID3D11VertexShader>& GetVertexShader();
	ComPtr<ID3D11InputLayout>& GetVertexLayout();

protected:
	ComPtr<ID3D11VertexShader> m_vertexShader;
	ComPtr<ID3D11InputLayout> m_vertexLayout;
};