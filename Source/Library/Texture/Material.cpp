#include "Texture/Material.h"

Material::Material()
	: AmbientColor(XMFLOAT3(0.0f, 0.0f, 0.0f))
	, DiffuseColor(XMFLOAT3(0.0f, 0.0f, 0.0f))
	, SpecularColor(XMFLOAT3(0.0f, 0.0f, 0.0f))
	, pDiffuse(nullptr)
	, pSpecular(nullptr)
{
}