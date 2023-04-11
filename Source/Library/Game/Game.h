#pragma once

#include "Common.h"

HRESULT InitWindow(_In_ HINSTANCE hInstance, _In_ INT nCmdShow);

HRESULT InitDevice();

void CleanupDevice();

void Render();

HRESULT CompileShaderFromFile(
	_In_	PCWSTR		pszFileName,	// FileName
	_In_	PCSTR		pszEntryPoint,	// EntryPoint
	_In_	PCSTR		pszShaderModel, // Shader target 
	_Outptr_ ID3DBlob** ppBlobOut		// ID3DBlob out	
);

struct SimpleVertex
{
	XMFLOAT3 Position;
	XMFLOAT2 Material;
};

struct CBView
{
	XMMATRIX View;
};
struct CBWorld
{
	XMMATRIX World;
};
struct CBProjection
{
	XMMATRIX Projection;
};