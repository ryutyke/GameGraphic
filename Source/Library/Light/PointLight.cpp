#include "Light/PointLight.h"


PointLight::PointLight(_In_ const XMFLOAT4& position, _In_  const XMFLOAT4& color)
	: m_position(position)
	, m_color(color)
{
}

const XMFLOAT4& PointLight::GetPosition() const
{
	return m_position;
}

const XMFLOAT4& PointLight::GetColor() const
{
	return m_color;
}

void PointLight::Update(_In_ FLOAT deltaTime)
{
}