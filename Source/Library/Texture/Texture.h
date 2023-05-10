#pragma once

#include "Common.h"

class Texture
{
public:
	Texture(const std::filesystem::path& filepath);
	~Texture();
	HRESULT Initialize(_In_ ID3D11Device* pDevice, _In_ ID3D11DeviceContext* pImmediateContext);
	ComPtr<ID3D11ShaderResourceView>& GetTextureResourceView();
	ComPtr<ID3D11SamplerState> GetSamplerState();
private:
	std::filesystem::path m_filePath;
	ComPtr<ID3D11ShaderResourceView> m_textureRV;
	ComPtr<ID3D11SamplerState> m_samplerLinear;
};

class Material
{
public:
	Material();
	~Material();
public:
	std::shared_ptr<Texture> pDiffuse;
	std::shared_ptr<Texture> pSpecular;
	
	XMFLOAT3 AmbientColor;
	XMFLOAT3 DiffuseColor;
	XMFLOAT3 SpecularColor;
};