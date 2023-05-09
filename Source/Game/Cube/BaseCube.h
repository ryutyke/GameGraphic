#pragma once

#include "Common.h"

#include "Renderer/DataTypes.h"
#include "Renderer/Renderable.h"

class BaseCube : public Renderable
{
public:
    BaseCube(_In_ const std::filesystem::path& textureFilePath);
    BaseCube(_In_ const XMFLOAT4& outputColor);
    ~BaseCube() = default;

    virtual HRESULT Initialize(_In_ ID3D11Device* pDevice, _In_ ID3D11DeviceContext* pImmediateContext) override;
    virtual void Update(_In_ FLOAT deltaTime) = 0;

    UINT GetNumVertices() const override;
    UINT GetNumIndices() const override;

protected:
    const SimpleVertex* getVertices() const override;
    const WORD* getIndices() const override;

    static constexpr const SimpleVertex VERTICES[] =
    {
        {.Position = XMFLOAT3(-1.0f, 1.0f, -1.0f), .TexCoord = XMFLOAT2(1.0f, 0.0f), .Normal = XMFLOAT3(0.0f, 1.0f, 0.0f) },
        {.Position = XMFLOAT3(1.0f, 1.0f, -1.0f), .TexCoord = XMFLOAT2(0.0f, 0.0f), .Normal = XMFLOAT3(0.0f, 1.0f, 0.0f) },
        {.Position = XMFLOAT3(1.0f, 1.0f,  1.0f), .TexCoord = XMFLOAT2(0.0f, 1.0f), .Normal = XMFLOAT3(0.0f, 1.0f, 0.0f) },
        {.Position = XMFLOAT3(-1.0f, 1.0f,  1.0f), .TexCoord = XMFLOAT2(1.0f, 1.0f), .Normal = XMFLOAT3(0.0f, 1.0f, 0.0f) },

        {.Position = XMFLOAT3(-1.0f, -1.0f, -1.0f), .TexCoord = XMFLOAT2(0.0f, 0.0f), .Normal = XMFLOAT3(0.0f, -1.0f, 0.0f) },
        {.Position = XMFLOAT3(1.0f, -1.0f, -1.0f), .TexCoord = XMFLOAT2(1.0f, 0.0f), .Normal = XMFLOAT3(0.0f, -1.0f, 0.0f) },
        {.Position = XMFLOAT3(1.0f, -1.0f,  1.0f), .TexCoord = XMFLOAT2(1.0f, 1.0f), .Normal = XMFLOAT3(0.0f, -1.0f, 0.0f) },
        {.Position = XMFLOAT3(-1.0f, -1.0f,  1.0f), .TexCoord = XMFLOAT2(0.0f, 1.0f), .Normal = XMFLOAT3(0.0f, -1.0f, 0.0f) },

        {.Position = XMFLOAT3(-1.0f, -1.0f,  1.0f), .TexCoord = XMFLOAT2(0.0f, 1.0f), .Normal = XMFLOAT3(-1.0f, 0.0f, 0.0f) },
        {.Position = XMFLOAT3(-1.0f, -1.0f, -1.0f), .TexCoord = XMFLOAT2(1.0f, 1.0f), .Normal = XMFLOAT3(-1.0f, 0.0f, 0.0f) },
        {.Position = XMFLOAT3(-1.0f,  1.0f, -1.0f), .TexCoord = XMFLOAT2(1.0f, 0.0f), .Normal = XMFLOAT3(-1.0f, 0.0f, 0.0f) },
        {.Position = XMFLOAT3(-1.0f,  1.0f,  1.0f), .TexCoord = XMFLOAT2(0.0f, 0.0f), .Normal = XMFLOAT3(-1.0f, 0.0f, 0.0f) },

        {.Position = XMFLOAT3(1.0f, -1.0f,  1.0f), .TexCoord = XMFLOAT2(1.0f, 1.0f), .Normal = XMFLOAT3(1.0f, 0.0f, 0.0f) },
        {.Position = XMFLOAT3(1.0f, -1.0f, -1.0f), .TexCoord = XMFLOAT2(0.0f, 1.0f), .Normal = XMFLOAT3(1.0f, 0.0f, 0.0f) },
        {.Position = XMFLOAT3(1.0f,  1.0f, -1.0f), .TexCoord = XMFLOAT2(0.0f, 0.0f), .Normal = XMFLOAT3(1.0f, 0.0f, 0.0f) },
        {.Position = XMFLOAT3(1.0f,  1.0f,  1.0f), .TexCoord = XMFLOAT2(1.0f, 0.0f), .Normal = XMFLOAT3(1.0f, 0.0f, 0.0f) },

        {.Position = XMFLOAT3(-1.0f, -1.0f, -1.0f), .TexCoord = XMFLOAT2(0.0f, 1.0f), .Normal = XMFLOAT3(0.0f, 0.0f, -1.0f) },
        {.Position = XMFLOAT3(1.0f, -1.0f, -1.0f), .TexCoord = XMFLOAT2(1.0f, 1.0f), .Normal = XMFLOAT3(0.0f, 0.0f, -1.0f) },
        {.Position = XMFLOAT3(1.0f,  1.0f, -1.0f), .TexCoord = XMFLOAT2(1.0f, 0.0f), .Normal = XMFLOAT3(0.0f, 0.0f, -1.0f) },
        {.Position = XMFLOAT3(-1.0f,  1.0f, -1.0f), .TexCoord = XMFLOAT2(0.0f, 0.0f), .Normal = XMFLOAT3(0.0f, 0.0f, -1.0f) },

        {.Position = XMFLOAT3(-1.0f, -1.0f, 1.0f), .TexCoord = XMFLOAT2(1.0f, 1.0f), .Normal = XMFLOAT3(0.0f, 0.0f, 1.0f) },
        {.Position = XMFLOAT3(1.0f, -1.0f, 1.0f), .TexCoord = XMFLOAT2(0.0f, 1.0f), .Normal = XMFLOAT3(0.0f, 0.0f, 1.0f) },
        {.Position = XMFLOAT3(1.0f,  1.0f, 1.0f), .TexCoord = XMFLOAT2(0.0f, 0.0f), .Normal = XMFLOAT3(0.0f, 0.0f, 1.0f) },
        {.Position = XMFLOAT3(-1.0f,  1.0f, 1.0f), .TexCoord = XMFLOAT2(1.0f, 0.0f), .Normal = XMFLOAT3(0.0f, 0.0f, 1.0f) },
    };
    static constexpr const UINT NUM_VERTICES = 24u;
    static constexpr const WORD INDICES[] =
    {
        3,1,0,
        2,1,3,

        6,4,5,
        7,4,6,

        11,9,8,
        10,9,11,

        14,12,13,
        15,12,14,

        19,17,16,
        18,17,19,

        22,20,21,
        23,20,22
    };
    static constexpr const UINT NUM_INDICES = 36u;
};