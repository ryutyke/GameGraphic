#pragma once

#include "Common.h"

#include "Cube/BaseCube.h"

class RotatingCube : public BaseCube
{
public:
    RotatingCube(_In_ const std::filesystem::path& textureFilePath);
    RotatingCube(_In_ const XMFLOAT4& outputColor);
    ~RotatingCube() = default;

    virtual void Update(_In_ FLOAT deltaTime) override;
};