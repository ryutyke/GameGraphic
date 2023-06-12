#include "Common.h"

#include "Game/Game.h"
#include "Cube/RotatingCube.h"
#include "Cube/Cube.h"
#include "Light/RotatingPointLight.h"
#include "Shader/SkinningVertexShader.h"

INT WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ INT nCmdShow)
{

	std::unique_ptr<Game> game = std::make_unique<Game>(L"2019102090 류승현 Assignment 4");

    // Phong Skinning
    std::shared_ptr<VertexShader> vertexShader = std::make_shared<VertexShader>(L"Shaders/Shader.fx", "VS", "vs_5_0");
    if (FAILED(game->GetRenderer()->AddVertexShader(L"VS", vertexShader)))
        return 0;

    std::shared_ptr<PixelShader> pixelShader = std::make_shared<PixelShader>(L"Shaders/Shader.fx", "PS", "ps_5_0");
    if (FAILED(game->GetRenderer()->AddPixelShader(L"PS", pixelShader)))
        return 0;

    std::shared_ptr<PixelShader> pixelSolidShader = std::make_shared<PixelShader>(L"Shaders/Shader.fx", "PSSolid", "ps_5_0");
    if (FAILED(game->GetRenderer()->AddPixelShader(L"PSSolid", pixelSolidShader)))
        return 0;

    // Rotating Point Light
    XMFLOAT4 color;
    XMStoreFloat4(&color, Colors::White);
    std::shared_ptr<RotatingPointLight> rotatingPointLight = std::make_shared<RotatingPointLight>(XMFLOAT4(0.0f, 0.0f, -15.0f, 1.0f), color);
    if (FAILED(game->GetRenderer()->AddPointLight(0u, rotatingPointLight)))
        return 0;

    //// Rotating Point Light Cube
    //std::shared_ptr<RotatingCube> rotatingLightCube = std::make_shared<RotatingCube>(color);
    //rotatingLightCube->Translate(XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f));
    //if (FAILED(game->GetRenderer()->AddRenderable(L"RotatingLightCube", rotatingLightCube)))
    //    return 0;
    //if (FAILED(game->GetRenderer()->SetVertexShaderOfRenderable(L"RotatingLightCube", L"VS")))
    //    return 0;
    //if (FAILED(game->GetRenderer()->SetPixelShaderOfRenderable(L"RotatingLightCube", L"PSSolid")))
    //    return 0;

    // Point Light
    XMStoreFloat4(&color, Colors::AntiqueWhite);
    std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>(
        XMFLOAT4(-5.77f, 5.77f, -5.77f, 1.0f), color);
    if (FAILED(game->GetRenderer()->AddPointLight(1u, pointLight)))
        return 0;

    //// Point Light Cube
    //std::shared_ptr<Cube> cube = std::make_shared<Cube>(color);
    //cube->Translate(XMVectorSet(-5.0f, 5.0f, -5.0f, 0.0f));
    //if (FAILED(game->GetRenderer()->AddRenderable(L"Cube", cube)))
    //    return 0;
    //if (FAILED(game->GetRenderer()->SetVertexShaderOfRenderable(L"Cube", L"VS")))
    //    return 0;
    //if (FAILED(game->GetRenderer()->SetPixelShaderOfRenderable(L"Cube", L"PSSolid")))
    //    return 0;

    // Cube
    std::shared_ptr<Cube> cube = std::make_shared<Cube>(color);
    if (FAILED(game->GetRenderer()->AddRenderable(L"Cube", cube)))
        return 0;
    if (FAILED(game->GetRenderer()->SetVertexShaderOfRenderable(L"Cube", L"VS")))
        return 0;
    if (FAILED(game->GetRenderer()->SetPixelShaderOfRenderable(L"Cube", L"PS")))
        return 0;

    // Material
    std::shared_ptr<Material> cubeMaterial = std::make_shared<Material>(L"CubeMaterial");
    cubeMaterial->pDiffuse = std::make_shared<Texture>(L"../../Data/Cube/diffuse.png");
    cubeMaterial->pNormal = std::make_shared<Texture>(L"../../Data/Cube/normal.png");

    cube->AddMaterial(cubeMaterial);

    if (FAILED(game->Initialize(hInstance, nCmdShow)))
        return 0;

    return game->Run();
}