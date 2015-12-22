// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>


// TODO: reference additional headers your program requires here
//DirectX 
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")
#include <atlbase.h>

#define BUFFER_WIDTH 1600
#define BUFFER_HEIGHT 900

#define FULLSCREEN (1 << 0)
#define STYLEMOD (WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)