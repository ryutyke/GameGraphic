#include "Cube/Cube.h"

Cube::Cube(_In_ const std::filesystem::path& textureFilePath)
    : BaseCube(textureFilePath)
{
}

Cube::Cube(_In_ const XMFLOAT4& outputColor)
    : BaseCube(outputColor)
{
}

void Cube::Update(_In_ FLOAT deltaTime)
{
}