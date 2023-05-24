#pragma once

#define NUM_LIGHTS (2)

#define MAX_NUM_BONES (256)
#define MAX_NUM_BONES_PER_VERTEX (16)

#include "Common.h"

struct SimpleVertex
{
	XMFLOAT3 Position;
	XMFLOAT2 TexCoord;
	XMFLOAT3 Normal;
};

struct InstanceData
{
	XMMATRIX Transformation;
};

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
TODO: Add AnimationData structure
-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

struct CBChangeOnCameraMovement
{
	XMMATRIX View;
	XMFLOAT4 CameraPosition;
};
struct CBChangeOnResize
{
	XMMATRIX Projection;
};
struct CBChangeEveryFrame
{
	XMMATRIX World;
	XMFLOAT4 OutputColor;
};
struct CBLights
{
	XMFLOAT4 LightPositions[NUM_LIGHTS];
	XMFLOAT4 LightColors[NUM_LIGHTS];
};

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
TODO: Add CBSkinning structure //¿Ï·á
-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

struct CBSkinning
{
	XMMATRIX BoneTransforms[MAX_NUM_BONES];
};

struct InputDirections
{
	BOOL bFront;
	BOOL bBack;
	BOOL bLeft;
	BOOL bRight;
};

struct AnimationData
{
	XMUINT4 aBoneIndices;
	XMFLOAT4 aBoneWeights;
};