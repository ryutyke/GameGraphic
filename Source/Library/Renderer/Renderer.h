#pragma once

#include "Common.h"

#include "Window/MainWindow.h"
#include "Renderer/Renderable.h"
#include "Light/PointLight.h"
#include "Shader/VertexShader.h"
#include "Shader/PixelShader.h"
#include "Camera/Camera.h"

class Renderer
{
public:
	Renderer();
	~Renderer() = default;

	HRESULT InitDevice(_In_ HWND hWnd);
	HRESULT AddRenderable(_In_ PCWSTR pszRenderableName, _In_ const std::shared_ptr<Renderable>& renderable);
	HRESULT AddPointLight(_In_ size_t index, _In_ const std::shared_ptr<PointLight>& pPointLight);
	HRESULT AddVertexShader(_In_ PCWSTR pszVertexShaderName, _In_ const std::shared_ptr<VertexShader>& vertexShader);
	HRESULT AddPixelShader(_In_ PCWSTR pszPixelShaderName, _In_ const std::shared_ptr<PixelShader>& pixelShader);

	void HandleInput(_In_ const InputDirections& directions, _In_ const MouseRelativeMovement& mouseRelativeMovement, const BOOL& mouseRightClick, _In_ FLOAT deltaTime);
	void Update(_In_ FLOAT deltaTime);
	void Render();

	HRESULT SetVertexShaderOfRenderable(_In_ PCWSTR pszRenderableName, _In_ PCWSTR pszVertexShaderName);
	HRESULT SetPixelShaderOfRenderable(_In_ PCWSTR pszRenderableName, _In_ PCWSTR pszPixelShaderName);

	D3D_DRIVER_TYPE GetDriverType() const;

private:
	D3D_DRIVER_TYPE m_driverType;
	D3D_FEATURE_LEVEL m_featureLevel;
	ComPtr<ID3D11Device> m_d3dDevice;
	ComPtr<ID3D11Device1> m_d3dDevice1;
	ComPtr<ID3D11DeviceContext> m_immediateContext;
	ComPtr<ID3D11DeviceContext1> m_immediateContext1;
	ComPtr<IDXGISwapChain> m_swapChain;
	ComPtr<IDXGISwapChain1> m_swapChain1;
	ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	ComPtr<ID3D11Texture2D> m_depthStencil;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;

	ComPtr<ID3D11Buffer> m_cbChangeOnResize;
	ComPtr<ID3D11Buffer> m_cbLights;

	Camera m_camera;
	XMMATRIX m_projection;

	std::unordered_map<PCWSTR, std::shared_ptr<Renderable>> m_renderables;
	std::shared_ptr<PointLight> m_aPointLights[NUM_LIGHTS];
	std::unordered_map<PCWSTR, std::shared_ptr<VertexShader>> m_vertexShaders;
	std::unordered_map<PCWSTR, std::shared_ptr<PixelShader>> m_pixelShaders;
};