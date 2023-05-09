#pragma once

#include "Common.h"

#include "Cube/BaseCube.h"

class Cube :public BaseCube
{
public:
    Cube(_In_ const std::filesystem::path& textureFilePath);
    Cube(_In_ const XMFLOAT4& outputColor);


    ~Cube() = default;

    virtual void Update(_In_ FLOAT deltaTime) override;
};