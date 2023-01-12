//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : ヒセイシン
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(11)				// テクスチャの数

#define CHECKMODE					(3)				// モードの数
#define BACKGROUND					(4)				// BGの数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/bg001.png",
	"data/TEXTURE/bg002.png",
	"data/TEXTURE/bg003.png",
	"data/TEXTURE/bg004.png",
	"data/TEXTURE/newgame.png",
	"data/TEXTURE/newgame_check.png",
	"data/TEXTURE/loadgame.png",
	"data/TEXTURE/loadgame_check.png",
	"data/TEXTURE/quit.png",
	"data/TEXTURE/quit_check.png",
	"data/TEXTURE/title.png",

};

static BOOL					g_Load = FALSE;
static TITLE				g_Title[BACKGROUND];


int checkmode = 0;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(void)
{
	ID3D11Device* pDevice = GetDevice();

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
	for (int i = 0; i < BACKGROUND; i++)
	{
		g_Title[i].w = TEXTURE_WIDTH * 1.6;
		g_Title[i].h = TEXTURE_HEIGHT;
		g_Title[i].pos = XMFLOAT3(TEXTURE_WIDTH /2, TEXTURE_HEIGHT/2, 0.0f);
		g_Title[i].scrl = 0;
	}
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
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
void UpdateTitle(void)
{
	g_Title[0].scrl += 0.002f;
	g_Title[1].scrl += 0.0015f;
	g_Title[2].scrl += 0.001f;
	g_Title[3].scrl += 0.001f;

	if (GetKeyboardTrigger(DIK_DOWN))
	{
		PlaySound(SOUND_LABEL_SE_button);
		checkmode++;
	}
	else if (GetKeyboardTrigger(DIK_UP))
	{
		PlaySound(SOUND_LABEL_SE_button);
		if (checkmode > 0)
		{
			checkmode--;
		}
		else
		{
			checkmode = CHECKMODE - 1;
		}
	}

	checkmode %= CHECKMODE;

	if (GetKeyboardTrigger(DIK_RETURN))
	{// Enter押したら、ステージを切り替える
		PlaySound(SOUND_LABEL_SE_button);
		switch (checkmode)
		{
		case 0:
			SetFade(FADE_OUT, MODE_TUTORIAL);
			break;
		case 1:
			//SetLoadGame(TRUE);
			SetFade(FADE_OUT, MODE_TUTORIAL);
			break;
		case 2:
			exit(0);
			break;
		}
	}




#ifdef _DEBUG	// デバッグ情報を表示する
	//PrintDebugProc("Player:↑ → ↓ ←　Space\n");

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
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

	// タイトルの背景を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer,
			g_Title[0].pos.x, g_Title[0].pos.y, g_Title[0].w, g_Title[0].h,
			g_Title[0].scrl, 0.0f, 1.0f, 1.0f,
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
			g_Title[1].pos.x, g_Title[1].pos.y, g_Title[1].w, g_Title[1].h,
			g_Title[1].scrl, 0.0f, 1.0f, 1.0f,
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
			g_Title[2].pos.x, g_Title[2].pos.y, g_Title[2].w, g_Title[2].h,
			g_Title[2].scrl, 0.0f, 1.0f, 1.0f,
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
			g_Title[3].pos.x, g_Title[3].pos.y, g_Title[3].w, g_Title[3].h,
			g_Title[3].scrl, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	switch (checkmode)
	{
	case 0:
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[4]);
		SetSpriteLeftTop(g_VertexBuffer, 650.0f, 200.0f, 300, 240, 0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[7]);
		SetSpriteLeftTop(g_VertexBuffer, 650.0f, 270.0f, 300, 240, 0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[9]);
		SetSpriteLeftTop(g_VertexBuffer, 650.0f, 340.0f, 300, 240, 0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
		break;
	case 1:
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[5]);
		SetSpriteLeftTop(g_VertexBuffer, 650.0f, 200.0f, 300, 240, 0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[6]);
		SetSpriteLeftTop(g_VertexBuffer, 650.0f, 270.0f, 300, 240, 0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[9]);
		SetSpriteLeftTop(g_VertexBuffer, 650.0f, 340.0f, 300, 240, 0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
		break;
	case 2:
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[5]);
		SetSpriteLeftTop(g_VertexBuffer, 650.0f, 200.0f, 300, 240, 0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[7]);
		SetSpriteLeftTop(g_VertexBuffer, 650.0f, 270.0f, 300, 240, 0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[8]);
		SetSpriteLeftTop(g_VertexBuffer, 650.0f, 340.0f, 300, 240, 0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
		break;
	}

	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[10]);
		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0, 960, 440, 0.0f, 0.0f, 1.0f, 1.0f);
		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
}



