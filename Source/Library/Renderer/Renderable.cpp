#include "Renderer/Renderable.h"

Renderable::Renderable(_In_ const XMFLOAT4& outputColor)
	: m_vertexBuffer()
	, m_indexBuffer()
	, m_normalBuffer()
	, m_cbChangeEveryFrame()
	, m_aMeshes()
	, m_aMaterials()
	, m_vertexShader()
	, m_pixelShader()
	, m_outputColor(outputColor)
	, m_world(XMMatrixIdentity())
	, m_bHasNormalMap(false)
	, m_aNormalData()
{
}

HRESULT Renderable::initialize(
	_In_ ID3D11Device* pDevice,
	_In_ ID3D11DeviceContext* pImmediateContext
)
{
	D3D11_BUFFER_DESC bd = {};
	{
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(SimpleVertex) * GetNumVertices();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
	}

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = getVertices();

	HRESULT hr = pDevice->CreateBuffer(&bd, &initData, m_vertexBuffer.GetAddressOf());

	if (FAILED(hr))
		return hr;

	if (m_aNormalData.empty())
	{
		calculateNormalMapVectors();
	}

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(NormalData) * GetNumVertices();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	initData.pSysMem = m_aNormalData.data();
	hr = pDevice->CreateBuffer(&bd, &initData, m_normalBuffer.GetAddressOf());

	if (FAILED(hr))
		return hr;

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * GetNumIndices();
	bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	initData.pSysMem = getIndices();

	hr = pDevice->CreateBuffer(&bd, &initData, m_indexBuffer.GetAddressOf());

	if (FAILED(hr))
		return hr;

	bd.ByteWidth = sizeof(CBChangeEveryFrame);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hr = pDevice->CreateBuffer(&bd, nullptr, m_cbChangeEveryFrame.GetAddressOf());
	if (FAILED(hr))
		return hr;

	for (auto it = m_aMaterials.begin(); it != m_aMaterials.end(); ++it)
	{
		HRESULT hr = it->get()->Initialize(pDevice, pImmediateContext);

		if (FAILED(hr))
			return hr;
	}

	return S_OK;
}

void Renderable::SetVertexShader(_In_ const std::shared_ptr<VertexShader>& vertexShader)
{
	m_vertexShader = vertexShader;
}

void Renderable::SetPixelShader(_In_ const std::shared_ptr<PixelShader>& pixelShader)
{
	m_pixelShader = pixelShader;
}

ComPtr<ID3D11VertexShader>& Renderable::GetVertexShader()
{
	return m_vertexShader->GetVertexShader();
}

ComPtr<ID3D11PixelShader>& Renderable::GetPixelShader()
{
	return m_pixelShader->GetPixelShader();
}

ComPtr<ID3D11InputLayout>& Renderable::GetVertexLayout()
{
	return m_vertexShader->GetVertexLayout();
}

ComPtr<ID3D11Buffer>& Renderable::GetVertexBuffer()
{
	return m_vertexBuffer;
}

ComPtr<ID3D11Buffer>& Renderable::GetIndexBuffer()
{
	return m_indexBuffer;
}

ComPtr<ID3D11Buffer>& Renderable::GetConstantBuffer()
{
	return m_cbChangeEveryFrame;
}

ComPtr<ID3D11Buffer>& Renderable::GetNormalBuffer()
{
	return m_normalBuffer;
}

const XMMATRIX& Renderable::GetWorldMatrix() const
{
	return m_world;
}

const XMFLOAT4& Renderable::GetOutputColor() const
{
	return m_outputColor;
}

BOOL Renderable::HasTexture() const
{
	return m_aMaterials.size() > 0;
}

const std::shared_ptr<Material>& Renderable::GetMaterial(UINT uIndex) const
{
	assert(uIndex < m_aMaterials.size());

	return m_aMaterials[uIndex];
}

const Renderable::BasicMeshEntry& Renderable::GetMesh(UINT uIndex) const
{
	assert(uIndex < m_aMeshes.size());

	return m_aMeshes[uIndex];
}

void Renderable::RotateX(_In_ FLOAT angle)
{
	m_world *= XMMatrixRotationX(angle);
}

void Renderable::RotateY(_In_ FLOAT angle)
{
	m_world *= XMMatrixRotationY(angle);
}

void Renderable::RotateZ(_In_ FLOAT angle)
{
	m_world *= XMMatrixRotationZ(angle);
}

void Renderable::RotateYInObjecteCoordinate(_In_ FLOAT angle, _In_ const XMVECTOR& offset)
{
	Translate(-offset);
	RotateY(angle);
	Translate(offset);
}

void Renderable::RotateRollPitchYaw(_In_ FLOAT roll, _In_ FLOAT pitch, _In_ FLOAT yaw)
{
	m_world *= XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
}

void Renderable::Scale(_In_ FLOAT scaleX, _In_ FLOAT scaleY, _In_ FLOAT scaleZ)
{
	m_world *= XMMatrixScaling(scaleX, scaleY, scaleZ);
}

void Renderable::Translate(_In_ const XMVECTOR& offset)
{
	m_world *= XMMatrixTranslationFromVector(offset);
}

UINT Renderable::GetNumMeshes() const
{
	return static_cast<UINT>(m_aMeshes.size());
}

UINT Renderable::GetNumMaterials() const
{
	return static_cast<UINT>(m_aMaterials.size());
}

void Renderable::calculateNormalMapVectors() {
	UINT uNumFaces = GetNumIndices() / 3;
	const SimpleVertex* aVertices = getVertices();
	const WORD* aIndices = getIndices();

	m_aNormalData.resize(GetNumVertices(), NormalData());

	XMFLOAT3 tangent, bitangent;

	for (int i = 0; i < uNumFaces; i++)
	{
		calculateTangentBitangent(aVertices[aIndices[i * 3]], aVertices[aIndices[i * 3 + 1]],
			aVertices[aIndices[i * 3 + 2]], tangent, bitangent);

		m_aNormalData[aIndices[i * 3]].Tangent = tangent;
		m_aNormalData[aIndices[i * 3]].Bitangent = bitangent;

		m_aNormalData[aIndices[i * 3 + 1]].Tangent = tangent;
		m_aNormalData[aIndices[i * 3 + 1]].Bitangent = bitangent;

		m_aNormalData[aIndices[i * 3 + 2]].Tangent = tangent;
		m_aNormalData[aIndices[i * 3 + 2]].Bitangent = bitangent;
	}
}

void Renderable::calculateTangentBitangent(_In_ const SimpleVertex& v1, _In_ const SimpleVertex& v2, _In_ const SimpleVertex& v3, _Out_
	XMFLOAT3& tangent, _Out_ XMFLOAT3& bitangent)
{
	XMFLOAT3 vector1, vector2;
	XMFLOAT2 tuVector, tvVector;
	// Calculate the two vectors for this face.
	vector1.x = v2.Position.x - v1.Position.x; vector1.y = v2.Position.y - v1.Position.y; vector1.z = v2.Position.z - v1.Position.z;
	vector2.x = v3.Position.x - v1.Position.x; vector2.y = v3.Position.y - v1.Position.y; vector2.z = v3.Position.z - v1.Position.z;
	// Calculate the tu and tv texture space vectors.
	tuVector.x = v2.TexCoord.x - v1.TexCoord.x; tvVector.x = v2.TexCoord.y - v1.TexCoord.y;
	tuVector.y = v3.TexCoord.x - v1.TexCoord.x; tvVector.y = v3.TexCoord.y - v1.TexCoord.y;
	// Calculate the denominator of the tangent/binormal equation.
	float den = 1.0f / (tuVector.x * tvVector.y - tuVector.y * tvVector.x);
	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tvVector.y * vector1.x - tvVector.x * vector2.x) * den;
	tangent.y = (tvVector.y * vector1.y - tvVector.x * vector2.y) * den;
	tangent.z = (tvVector.y * vector1.z - tvVector.x * vector2.z) * den;
	bitangent.x = (tuVector.x * vector2.x - tuVector.y * vector1.x) * den;
	bitangent.y = (tuVector.x * vector2.y - tuVector.y * vector1.y) * den;
	bitangent.z = (tuVector.x * vector2.z - tuVector.y * vector1.z) * den;
	// Calculate the length of this normal.
	float length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));
	// Normalize the normal and then store it
	tangent.x = tangent.x / length; tangent.y = tangent.y / length; tangent.z = tangent.z / length;
	// Calculate the length of this normal.
	length = sqrt((bitangent.x * bitangent.x) + (bitangent.y * bitangent.y) + (bitangent.z * bitangent.z));
	// Normalize the normal and then store it
	bitangent.x = bitangent.x / length; bitangent.y = bitangent.y / length; bitangent.z = bitangent.z / length;
}

void Renderable::AddMaterial(_In_ const std::shared_ptr<Material>& material)
{
	m_aMaterials.push_back(material);
}

HRESULT Renderable::SetMaterialOfMesh(_In_ const UINT uMeshIndex, _In_ const UINT uMaterialIndex)
{
	if (uMeshIndex >= m_aMeshes.size() || uMaterialIndex >= m_aMaterials.size())
	{
		return E_FAIL;
	}

	m_aMeshes[uMeshIndex].uMaterialIndex = uMaterialIndex;

	if (m_aMaterials[uMeshIndex]->pNormal)
	{
		m_bHasNormalMap = true;
	}

	return S_OK;
}

BOOL Renderable::HasNormalMap() const
{
	return m_bHasNormalMap;
}