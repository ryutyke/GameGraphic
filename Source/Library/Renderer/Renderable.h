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

	void AddMaterial(_In_ const std::shared_ptr<Material>& material);
	HRESULT SetMaterialOfMesh(_In_ const UINT uMeshIndex, _In_ const UINT uMaterialIndex);

	ComPtr<ID3D11VertexShader>& GetVertexShader();
	ComPtr<ID3D11PixelShader>& GetPixelShader();
	ComPtr<ID3D11InputLayout>& GetVertexLayout();

	ComPtr<ID3D11Buffer>& GetVertexBuffer();
	ComPtr<ID3D11Buffer>& GetIndexBuffer();
	ComPtr<ID3D11Buffer>& GetConstantBuffer();
	ComPtr<ID3D11Buffer>& GetNormalBuffer();


	const XMMATRIX& GetWorldMatrix() const;
	const XMFLOAT4& GetOutputColor() const;
	BOOL HasTexture() const;

	const std::shared_ptr<Material>& GetMaterial(UINT uIndex) const;
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
	BOOL HasNormalMap() const;

protected:
	const virtual SimpleVertex* getVertices() const = 0;
	virtual const WORD* getIndices() const = 0;

	HRESULT initialize(
		_In_ ID3D11Device* pDevice, 
		_In_ ID3D11DeviceContext* pImmediateContext
	);

	void calculateNormalMapVectors();
	void calculateTangentBitangent(_In_ const SimpleVertex& v1, _In_ const SimpleVertex& v2, _In_ const SimpleVertex& v3, _Out_
		XMFLOAT3& tangent, _Out_ XMFLOAT3& bitangent);

	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
	ComPtr<ID3D11Buffer> m_cbChangeEveryFrame;
	ComPtr<ID3D11Buffer> m_normalBuffer;


	std::vector<BasicMeshEntry> m_aMeshes;
	std::vector<std::shared_ptr<Material>> m_aMaterials;
	std::vector<NormalData> m_aNormalData;

	std::shared_ptr<VertexShader> m_vertexShader;
	std::shared_ptr<PixelShader> m_pixelShader;

	XMFLOAT4 m_outputColor;
	XMMATRIX m_world;
	BOOL m_bHasNormalMap;
};