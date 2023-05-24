#pragma once

#include "Common.h"

class Texture
{
public:
	Texture() = delete;
	Texture(_In_ const std::filesystem::path& filePath);
	virtual ~Texture() = default;

	virtual HRESULT Initialize(_In_ ID3D11Device* pDevice, _In_ ID3D11DeviceContext* pImmediateContext);

	ComPtr<ID3D11ShaderResourceView>& GetTextureResourceView();
	ComPtr<ID3D11SamplerState>& GetSamplerState();

private:
	std::filesystem::path m_filePath;
	ComPtr<ID3D11ShaderResourceView> m_textureRV;
	ComPtr<ID3D11SamplerState> m_samplerLinear;
};