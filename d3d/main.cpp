#include <d3d9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define WINDOW_CLASS "UGPDX"       // ����������
#define WINDOW_NAME "Blank D3D Window"

#define FVF D3DFVF_XYZRHW | D3DFVF_DIFFUSE
struct VERTEX
{
	float x, y, z, rhw;
	DWORD color;
};

// ��������
bool InitializeD3D(HWND hWnd, bool fullscreen);
void RenderScene();
void Shutdown();

// ����ȫ�ֱ��� Direct3D������豸����
// ��ʼ��ΪNULL
LPDIRECT3D9 g_D3D = NULL;
LPDIRECT3DDEVICE9 g_D3DDevice = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;

// ���崰����Ϣ������
// ������ESC�˳�����
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

// �������
// WinMain����
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

		// ������Ϣѭ��
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

	// �ͷ�������Դ
	Shutdown();

	// Unregister our window.
	UnregisterClass(WINDOW_CLASS, wc.hInstance);
	return 0;
}

// ��ʼ��Direct3D������Direct3D�����豸����
// InitializeD3D����
bool InitializeD3D(HWND hWnd, bool fullscreen)
{
	D3DDISPLAYMODE displayMode;         // �Կ���ʾģʽ

										// ����D3D���󣬻����ӿ�g_D3D
										// ����������D3D_SDK_VERSION
	g_D3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_D3D == NULL) { return false; }

	// ��ȡ�Կ���ʾģʽ
	if (FAILED(g_D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode)))
	{
		return false;
	}

	// ����D3DPRESENT_PARAMETERS�ṹ
	// �ýṹ���ڶ���Direct3D���ڵ���ʾ��Ϣ
	// �����������ô����Ĵ��ڵ�������Ⱥ͸߶ȡ�ˢ���ʡ���ʾģʽ���Ƿ�ȫ�����ͺ�̨������Ŀ��
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

	// ����D3D�豸����ȡ��ӿ�g_D3D
	if (FAILED(g_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_D3DDevice)))
	{
		return false;
	}
	return true;
}

// ��ʾ��������
void RenderScene()
{
	// Clear������D3D�豸�����һ����Ա����
	// ���������̨����
	g_D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	// Begin the scene. Start rendering
	g_D3DDevice->BeginScene();

	// End the scen. Stop rendering
	g_D3DDevice->EndScene();

	// Display the scene.
	g_D3DDevice->Present(NULL, NULL, NULL, NULL);
}

// ���Direct3��Դ������
void Shutdown()
{
	if (g_D3DDevice != NULL) { g_D3DDevice->Release(); }
	if (g_D3D != NULL) { g_D3D->Release(); }

	g_D3DDevice = NULL;
	g_D3D = NULL;
}