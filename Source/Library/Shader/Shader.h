#pragma once

#include "Common.h"

class Shader
{
public:
	Shader(_In_ PCWSTR pszFileName, _In_ PCSTR pszEntryPoint, _In_ PCSTR  pszShaderModel);
	virtual ~Shader() = default;

	virtual HRESULT Initialize(_In_ ID3D11Device* pDevice) = 0;
	PCWSTR GetFileName() const;

protected:
	HRESULT compile(_Outptr_ ID3DBlob** ppOutBlob);

	PCWSTR m_pszFileName;
	PCSTR m_pszEntryPoint;
	PCSTR m_pszShaderModel;
};