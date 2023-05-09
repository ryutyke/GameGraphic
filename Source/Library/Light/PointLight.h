#pragma once

#include "Common.h"

#include "Shader/PixelShader.h"

class PointLight
{
public:
	PointLight() = delete;
	PointLight(_In_ const XMFLOAT4& position, _In_ const XMFLOAT4& color);
	virtual ~PointLight() = default;

	const XMFLOAT4& GetPosition() const;
	const XMFLOAT4& GetColor() const;

	virtual void Update(_In_ FLOAT deltaTime);

protected:
	XMFLOAT4 m_position;
	XMFLOAT4 m_color;
};