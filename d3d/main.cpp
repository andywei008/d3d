#include<d3d9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define WINDOW_CLASS L"UGPDX"             // �궨�崰��������
#define WINDOW_NAME L"Blank D3D Window"

// ��������
bool InitializeD3D(HWND hWnd, bool  fullscreen);
void RenderScene();
void Shutdown();