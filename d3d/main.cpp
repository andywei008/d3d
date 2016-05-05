#include<d3d9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define WINDOW_CLASS L"UGPDX"             // 宏定义窗口类名称
#define WINDOW_NAME L"Blank D3D Window"

// 函数声明
bool InitializeD3D(HWND hWnd, bool  fullscreen);
void RenderScene();
void Shutdown();