#pragma once

#include "Common.h"

#include "Renderer/DataTypes.h"

class Camera
{
public:
	Camera() = delete;
	Camera(_In_ const XMVECTOR& position);
	virtual ~Camera() = default;

	const XMVECTOR& GetEye() const;
	const XMVECTOR& GetAt() const;
	const XMVECTOR& GetUp() const;
	const XMMATRIX& GetView() const;
	ComPtr<ID3D11Buffer>& GetConstantBuffer();

	virtual void HandleInput(_In_ const InputDirections& directions, _In_ const MouseRelativeMovement& mouseRelativeMovement, const BOOL& mouseRightClick, _In_ FLOAT deltaTime);
	virtual HRESULT Initialize(_In_ ID3D11Device* device, _In_ ID3D11DeviceContext* pImmediateContext);
	virtual void Update(_In_ FLOAT deltaTime);

protected:
	static constexpr const XMVECTORF32 DEFAULT_FORWARD = { 0.0f, 0.0f, 1.0f, 0.0f };
	static constexpr const XMVECTORF32 DEFAULT_RIGHT = { 1.0f, 0.0f, 0.0f, 0.0f };
	static constexpr const XMVECTORF32 DEFAULT_UP = { 0.0f, 1.0f, 0.0f, 0.0f };

	ComPtr<ID3D11Buffer> m_cbChangeOnCameraMovement;

	FLOAT m_yaw;
	FLOAT m_pitch;

	FLOAT m_moveLeftRight;
	FLOAT m_moveBackForward;
	FLOAT m_moveUpDown;

	FLOAT m_movementSpeed;
	FLOAT m_rotationSpeed;

	XMVECTOR m_cameraForward;
	XMVECTOR m_cameraRight;
	XMVECTOR m_cameraUp;

	XMVECTOR m_eye;
	XMVECTOR m_at;
	XMVECTOR m_up;

	XMMATRIX m_rotation;
	XMMATRIX m_view;
};