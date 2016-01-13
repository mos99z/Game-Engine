// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers


#include <string>
#include <vector>
#include <cstdint>
#include <fstream>
// TODO: reference additional headers your program requires here
#include <d3d11.h>
#pragma comment(lib,"d3d11.lib")
#include <atlbase.h>
#include <Windows.h>
#define RENDERERDLL_EXPORTS
#define RENDERCONTEXTDLL_EXPORTS
#define RENDERSETDLL_EXPORTS
#define RENDERNODEDLL_EXPORTS
#define RENDERMATERIALDLL_EXPORTS
#define RENDERSHAPEDLL_EXPORTS
#define CAMERADLL_EXPORTS

