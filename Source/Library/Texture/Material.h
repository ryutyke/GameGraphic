#pragma once

#include "Common.h"

#include "Texture/Texture.h"

class Material
{
public:
	Material();
	virtual ~Material() = default;
	XMFLOAT3 AmbientColor;
	XMFLOAT3 DiffuseColor;
	XMFLOAT3 SpecularColor;

public:
	std::shared_ptr<Texture> pDiffuse;
	std::shared_ptr<Texture> pSpecular;
};