#pragma warning( push )  
#pragma warning(disable:4005)
#include <d3dx9.h>
#pragma warning( pop )  
#include "ImGui/imgui.h"

class CGuiRenderer
{

private:

	struct Vertex
    {
        D3DXVECTOR3 pos;
        D3DCOLOR    col;
        D3DXVECTOR2 uv;
    };

private:

	static IDirect3DVertexBuffer9*	s_pVB;
	static IDirect3DIndexBuffer9*	s_pIB;
	static IDirect3DTexture9*		s_pFontTexture;
	static IDirect3DTexture9*		s_pWhiteTexture;
	static ID3DXEffect*				s_pShader ;

    static const UINT FVF		= (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1);
    static const UINT VB_SIZE	= 100000;
	static const UINT IB_SIZE	= 200000;

private:
	static void RenderCallback(ImDrawData* draw_data);
	static bool CreateFontTexture();

public:
	static IDirect3DDevice9*		s_pDevice;

	static bool Initialize(IDirect3DDevice9* device, int width, int height);
	static void CleanUp();
	static void Update(float dt);
	static void Render();
	static void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

