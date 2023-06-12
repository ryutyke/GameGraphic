#pragma once

#include "Common.h"

#include "Texture/Texture.h"

class Material
{
public:
	Material() = delete;
	Material(_In_ std::wstring szName);
	virtual ~Material() = default;

	virtual HRESULT Initialize(_In_ ID3D11Device* pDevice, _In_ ID3D11DeviceContext* pImmediateContext);

	std::wstring GetName() const;

private:
	std::wstring m_szName;

public:
	std::shared_ptr<Texture> pDiffuse;
	std::shared_ptr<Texture> pSpecular;
	std::shared_ptr<Texture> pNormal;
};