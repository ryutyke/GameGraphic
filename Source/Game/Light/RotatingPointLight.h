#pragma once

#include "Common.h"

#include "Light/PointLight.h"

class RotatingPointLight final : public PointLight
{
public:
    RotatingPointLight() = delete;
    RotatingPointLight(_In_ const XMFLOAT4& position, _In_ const XMFLOAT4& color);
    virtual ~RotatingPointLight() = default;

    virtual void Update(_In_ FLOAT deltaTime) override;
};