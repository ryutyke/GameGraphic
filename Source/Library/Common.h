#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <wrl.h> // for comptr

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")

#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <directxcolors.h>

#include <iostream>
#include <string>
#include <filesystem>
#include <unordered_map>

using namespace Microsoft::WRL;
using namespace DirectX;