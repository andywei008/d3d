#include <d3d9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define WINDOW_CLASS "UGPDX"       // 窗口类名称
#define WINDOW_NAME "Blank D3D Window"

#define FVF D3DFVF_XYZRHW | D3DFVF_DIFFUSE
struct VERTEX
{
	float x, y, z, rhw;
	DWORD color;
};

// 函数声明
bool InitializeD3D(HWND hWnd, bool fullscreen);
void RenderScene();
void Shutdown();

// 声明全局变量 Direct3D对象和设备对象
// 初始化为NULL
LPDIRECT3D9 g_D3D = NULL;
LPDIRECT3DDEVICE9 g_D3DDevice = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;

// 定义窗口消息处理函数
// 允许按下ESC退出程序
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYUP:
		if (wParam == VK_ESCAPE) { PostQuitMessage(0); }
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

HRESULT InitVB()
{
	VERTEX vertexes[] = {
		{ 0.0f, 200.0f, 1.0f, 1.0f, 0xff112233 },
		{ 150.0f, 0.0f, 1.0f, 1.0f, 0xff224488 },
		{ 300.0f, 200.0f, 1.0f, 1.0f, 0xff789034 }
	};

	if (FAILED(g_D3DDevice->CreateVertexBuffer(sizeof(vertexes)
		, 0
		, FVF
		, D3DPOOL_DEFAULT
		, &g_pVB
		, NULL)))
	{
		return E_FAIL;
	}

	void *pvertexes = NULL;
	if (FAILED(g_pVB->Lock(0, sizeof(vertexes), (void**)&pvertexes, 0)))
	{
		return E_FAIL;
	}
	memcpy(pvertexes, vertexes, sizeof(vertexes));
	g_pVB->Unlock();

	return S_OK;
}

// 程序入口
// WinMain函数
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE prevhInst, LPSTR cmdLine, int show)
{
	// Register the window class
	WNDCLASSEX wc =
	{ sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		WINDOW_CLASS, NULL };
	RegisterClassEx(&wc);

	// Create the application's window
	HWND hWnd = CreateWindow(WINDOW_CLASS, WINDOW_NAME, WS_OVERLAPPEDWINDOW,
		100, 100, 640, 480, GetDesktopWindow(), NULL,
		wc.hInstance, NULL);

	// Initialize Direct3D
	if (InitializeD3D(hWnd, false))
	{
		// Show the window
		ShowWindow(hWnd, SW_SHOWDEFAULT);
		UpdateWindow(hWnd);

		// 进入消息循环
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0u, 0u, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else { RenderScene(); }
		}
	}

	// 释放所有资源
	Shutdown();

	// Unregister our window.
	UnregisterClass(WINDOW_CLASS, wc.hInstance);
	return 0;
}

// 初始化Direct3D，创建Direct3D对象及设备对象
// InitializeD3D函数
bool InitializeD3D(HWND hWnd, bool fullscreen)
{
	D3DDISPLAYMODE displayMode;         // 显卡显示模式

										// 创建D3D对象，获得其接口g_D3D
										// 参数必须是D3D_SDK_VERSION
	g_D3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_D3D == NULL) { return false; }

	// 获取显卡显示模式
	if (FAILED(g_D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode)))
	{
		return false;
	}

	// 定义D3DPRESENT_PARAMETERS结构
	// 该结构用于定义Direct3D窗口的显示信息
	// 这样可以设置创建的窗口的期望宽度和高度、刷新率、显示模式（是否全屏）和后台缓存数目等
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	if (fullscreen)
	{
		d3dpp.Windowed = FALSE;
		d3dpp.BackBufferWidth = 640;
		d3dpp.BackBufferHeight = 480;
	}
	else
	{
		d3dpp.Windowed = TRUE;
	}
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = displayMode.Format;

	// 创建D3D设备，获取其接口g_D3D
	if (FAILED(g_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_D3DDevice)))
	{
		return false;
	}
	return true;
}

// 显示窗口内容
void RenderScene()
{
	// Clear函数是D3D设备对象的一个成员函数
	// 用于清除后台缓存
	g_D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	// Begin the scene. Start rendering
	g_D3DDevice->BeginScene();

	// End the scen. Stop rendering
	g_D3DDevice->EndScene();

	// Display the scene.
	g_D3DDevice->Present(NULL, NULL, NULL, NULL);
}

// 清除Direct3资源及对象
void Shutdown()
{
	if (g_D3DDevice != NULL) { g_D3DDevice->Release(); }
	if (g_D3D != NULL) { g_D3D->Release(); }

	g_D3DDevice = NULL;
	g_D3D = NULL;
}