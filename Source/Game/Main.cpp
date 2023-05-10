#include "Common.h"

#include "Cube/Cube.h"
#include "Cube/RotatingCube.h"
#include "Light/RotatingPointLight.h"
#include "Model/Model.h"
#include "Game/Game.h"

INT WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ INT nCmdShow)
{

	std::unique_ptr<Game> game = std::make_unique<Game>(L"2019102090 류승현 LAB 08");

    // Phong Lighting Shaders
    std::shared_ptr<VertexShader> vertexShader = std::make_shared<VertexShader>(L"Shaders/Shader.fx", "VS", "vs_5_0");
    if (FAILED(game->GetRenderer()->AddVertexShader(L"VS", vertexShader)))
        return 0;

    std::shared_ptr<PixelShader> pixelShader = std::make_shared<PixelShader>(L"Shaders/Shader.fx", "PS", "ps_5_0");
    if (FAILED(game->GetRenderer()->AddPixelShader(L"PS", pixelShader)))
        return 0;

    std::shared_ptr<PixelShader> pixelSolidShader = std::make_shared<PixelShader>(L"Shaders/Shader.fx", "PSSolid", "ps_5_0");
    if (FAILED(game->GetRenderer()->AddPixelShader(L"PSSolid", pixelSolidShader)))
        return 0;

    // Point Light
    XMFLOAT4 color;
    XMStoreFloat4(&color, Colors::AntiqueWhite);
    std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>(XMFLOAT4(-5.77f, 5.77f, -5.77f, 1.0f), color);

    if (FAILED(game->GetRenderer()->AddPointLight(0u, pointLight)))
        return 0;

    // Point Light Cube
    std::shared_ptr<Cube> lightCube = std::make_shared<Cube>(color);
    lightCube->Translate(XMVectorSet(-5.77f, 5.77f, -5.77f, 0.0f));
    if (FAILED(game->GetRenderer()->AddRenderable(L"LightCube", lightCube)))
        return 0;
    if (FAILED(game->GetRenderer()->SetVertexShaderOfRenderable(L"LightCube", L"VS")))
        return 0;
    if (FAILED(game->GetRenderer()->SetPixelShaderOfRenderable(L"LightCube", L"PSSolid")))
        return 0;

    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
    TODO1: Rotating Point Light
    -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
    XMFLOAT4 color1;
    XMStoreFloat4(&color1, Colors::IndianRed);
    std::shared_ptr<RotatingPointLight> pointLight1 = std::make_shared<RotatingPointLight>(XMFLOAT4(0.0f, 0.0f, -5.0f, 1.0f), color1);

    if (FAILED(game->GetRenderer()->AddPointLight(1u, pointLight1)))
        return 0;

    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
    TODO2: Rotating Point Light Cube
    -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
    std::shared_ptr<RotatingCube> lightCube1 = std::make_shared<RotatingCube>(color1);
    lightCube1->Translate(XMVectorSet(0.0f, 0.0f, -5.0f, 1.0f));
    if (FAILED(game->GetRenderer()->AddRenderable(L"LightCube1", lightCube1)))   
        return 0;
    if (FAILED(game->GetRenderer()->SetVertexShaderOfRenderable(L"LightCube1", L"VS")))
        return 0;
    if (FAILED(game->GetRenderer()->SetPixelShaderOfRenderable(L"LightCube1", L"PSSolid")))
        return 0;

    //// Phong Cube
    //std::shared_ptr<Cube> phongCube = std::make_shared<Cube>(L"seafloor.dds");
    //if (FAILED(game->GetRenderer()->AddRenderable(L"PhongCube", phongCube)))
    //    return 0;
    //if (FAILED(game->GetRenderer()->SetVertexShaderOfRenderable(L"PhongCube", L"VS")))
    //    return 0;
    //if (FAILED(game->GetRenderer()->SetPixelShaderOfRenderable(L"PhongCube", L"PS")))
    //    return 0;

    std::shared_ptr<Model> spider = std::make_shared<Model>(L"../../Data/Stone/Stone.obj");
    spider->Scale(0.5f, 0.5f, 0.5f);
    if (FAILED(game->GetRenderer()->AddRenderable(L"Spider", spider)))
        return 0;
    if (FAILED(game->GetRenderer()->SetVertexShaderOfRenderable(L"Spider", L"VS")))
        return 0;
    if (FAILED(game->GetRenderer()->SetPixelShaderOfRenderable(L"Spider", L"PS")))
        return 0;

    if (FAILED(game->Initialize(hInstance, nCmdShow)))
        return 0;

    return game->Run();
}