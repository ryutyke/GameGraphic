#pragma once

#include "Common.h"

#include "Shader/PixelShader.h"
#include "Shader/VertexShader.h"

#include "Renderer/DataTypes.h"

#include "Texture/Material.h"

class Renderable
{
public:
	static constexpr const UINT INVALID_MATERIAL = (0xFFFFFFFF);

protected:
	struct BasicMeshEntry
	{
		BasicMeshEntry()
			: uNumIndices(0u)
			, uBaseVertex(0u)
			, uBaseIndex(0u)
			, uMaterialIndex(INVALID_MATERIAL)
		{}

		UINT uNumIndices;
		UINT uBaseVertex;
		UINT uBaseIndex;
		UINT uMaterialIndex;
	};

public:
	Renderable(_In_ const XMFLOAT4& outputColor);
	Renderable() = default;
	virtual ~Renderable() = default;

	virtual HRESULT Initialize(_In_ ID3D11Device* pDevice, _In_ ID3D11DeviceContext* pImmediateContext) = 0;
	virtual void Update(_In_ FLOAT deltaTime) = 0;

	void SetVertexShader(_In_ const std::shared_ptr<VertexShader>& vertexShader);
	void SetPixelShader(_In_ const std::shared_ptr<PixelShader>& pixelShader);

	ComPtr<ID3D11VertexShader>& GetVertexShader();
	ComPtr<ID3D11PixelShader>& GetPixelShader();
	ComPtr<ID3D11InputLayout>& GetVertexLayout();

	ComPtr<ID3D11Buffer>& GetVertexBuffer();
	ComPtr<ID3D11Buffer>& GetIndexBuffer();
	ComPtr<ID3D11Buffer>& GetConstantBuffer();

	const XMMATRIX& GetWorldMatrix() const;
	const XMFLOAT4& GetOutputColor() const;
	BOOL HasTexture() const;

	const Material& GetMaterial(UINT uIndex) const;
	const BasicMeshEntry& GetMesh(UINT uIndex) const;

	void RotateX(_In_ FLOAT angle);
	void RotateY(_In_ FLOAT angle);
	void RotateZ(_In_ FLOAT angle);
	void RotateYInObjecteCoordinate(_In_ FLOAT angle, _In_ const XMVECTOR& offset);
	void RotateRollPitchYaw(_In_ FLOAT roll, _In_ FLOAT pitch, _In_ FLOAT yaw);
	void Scale(_In_ FLOAT scaleX, _In_ FLOAT scaleY, _In_ FLOAT scaleZ);
	void Translate(_In_ const XMVECTOR& offset);

	virtual UINT GetNumVertices() const = 0;
	virtual UINT GetNumIndices() const = 0;

	UINT GetNumMeshes() const;
	UINT GetNumMaterials() const;
protected:
	const virtual SimpleVertex* getVertices() const = 0;
	virtual const WORD* getIndices() const = 0;

	HRESULT initialize(
		_In_ ID3D11Device* pDevice, 
		_In_ ID3D11DeviceContext* pImmediateContext
	);

	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
	ComPtr<ID3D11Buffer> m_cbChangeEveryFrame;

	std::vector<BasicMeshEntry> m_aMeshes;
	std::vector<Material> m_aMaterials;

	std::shared_ptr<VertexShader> m_vertexShader;
	std::shared_ptr<PixelShader> m_pixelShader;

	XMFLOAT4 m_outputColor;
	XMMATRIX m_world;
};