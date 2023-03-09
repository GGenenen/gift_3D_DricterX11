//=============================================================================
//
// リザルト画面処理 [result.cpp]
// Author : ヒセイシン
//
//=============================================================================
#include "result.h"
#include "input.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(5)			// テクスチャの数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/game clear.png",
	//"data/TEXTURE/bg002.png",
	//"data/TEXTURE/bg003.png",
	//"data/TEXTURE/bg004.png",
	//"data/TEXTURE/result.png",
};

static BOOL						g_Load = FALSE;
static RESULT					g_Result[TEXTURE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResult(void)
{
	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// 変数の初期化
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Result[i].w = TEXTURE_WIDTH * 1.6;
		g_Result[i].h = TEXTURE_HEIGHT;
		g_Result[i].pos = XMFLOAT3(TEXTURE_WIDTH / 2, TEXTURE_HEIGHT / 2, 0.0f);
		g_Result[i].scrl = 0;
	}

	// BGM再生
	g_Result[4].pos = XMFLOAT3(0.0f, 200.0f, 0.0f);

	g_Result[4].h = 250;


	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{

	g_Result[0].scrl += 0.002f;
	g_Result[1].scrl += 0.0015f;
	g_Result[2].scrl += 0.001f;
	g_Result[3].scrl += 0.001f;
	g_Result[4].scrl += 0.001f;

	if (g_Result[4].scrl >= 1.0f)
	{
		SetFade(FADE_OUT, MODE_TITLE);
	}
	
	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonPressed(0, BUTTON_A))
	{
		SetFade(FADE_OUT, MODE_TITLE);
	}


#ifdef _DEBUG	// デバッグ情報を表示する
	
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);


	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer,
			g_Result[0].pos.x, g_Result[0].pos.y, g_Result[0].w, g_Result[0].h,
			g_Result[0].scrl, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
	// タイトルの背景を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer,
			g_Result[1].pos.x, g_Result[1].pos.y, g_Result[1].w, g_Result[1].h,
			g_Result[1].scrl, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
	// タイトルの背景を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer,
			g_Result[2].pos.x, g_Result[2].pos.y, g_Result[2].w, g_Result[2].h,
			g_Result[2].scrl, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
	// タイトルの背景を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer,
			g_Result[3].pos.x, g_Result[3].pos.y, g_Result[3].w, g_Result[3].h,
			g_Result[3].scrl, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
	// タイトルの背景を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[4]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer,
			g_Result[4].pos.x, g_Result[4].pos.y, g_Result[4].w, g_Result[4].h,
			g_Result[4].scrl, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

}




