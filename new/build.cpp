//=============================================================================
//
// エネミーモデル処理 [enemy.cpp]
// Author : ヒセイシン
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "build.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define	VALUE_MOVE			(5.0f)						// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// 回転量

#define BUILD_SHADOW_SIZE	(0.4f)						// 影の大きさ
#define BUILD_OFFSET_Y		(7.0f)						// エネミーの足元をあわせる

#define	MODEL_0			"data/MODEL/enemy.obj"		// 読み込むモデル名


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
//static char* g_ModleName[] =
//{
//	"data/MODEL/tate_01.obj"
//};


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static BUILD			g_Build[MAX_BUILD];				// エネミー




int g_Build_load = 0;

float size = 1.0f;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBuild(void)
{

	for (int i = 0; i < MAX_BUILD; i++)
	{
		g_Build[i].load = true;

	}

	LoadModel(MODEL_0, &g_Build[0].model);
	LoadModel(MODEL_0, &g_Build[1].model);
	LoadModel(MODEL_0, &g_Build[2].model);
	LoadModel(MODEL_0, &g_Build[3].model);

	g_Build[0].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Build[0].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Build[0].scl = XMFLOAT3(size, size, size);

	g_Build[1].pos = XMFLOAT3(0.0f, 0.0f, -200.0f);
	g_Build[1].rot = XMFLOAT3(0.0f, XM_PI / 2, 0.0f);
	g_Build[1].scl = XMFLOAT3(size, size, size);

	g_Build[2].pos = XMFLOAT3(-200.0f, 0.0f, -200.0f);
	g_Build[2].rot = XMFLOAT3(0.0f, XM_PI, 0.0f);
	g_Build[2].scl = XMFLOAT3(size, size, size);

	g_Build[3].pos = XMFLOAT3(-200.0f, 0.0f, 0.0f);
	g_Build[3].rot = XMFLOAT3(0.0f, XM_PI + XM_PI / 2, 0.0f);
	g_Build[3].scl = XMFLOAT3(size, size, size);


	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Build[0].model, &g_Build[0].diffuse[0]);


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBuild(void)
{

	for (int i = 0; i < MAX_BUILD; i++)
	{
		if (g_Build[i].load)
		{
			UnloadModel(&g_Build[i].model);
			g_Build[i].load = false;
		}
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBuild(void)
{




#ifdef _DEBUG


#endif


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBuild(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_BUILD; i++)
	{
		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Build[i].scl.x, g_Build[i].scl.y, g_Build[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Build[i].rot.x, g_Build[i].rot.y + XM_PI, g_Build[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Build[i].pos.x, g_Build[i].pos.y, g_Build[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Build[i].mtxWorld, mtxWorld);


		// モデル描画
		DrawModel(&g_Build[i].model);
	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// エネミーの取得
//=============================================================================
BUILD* GetBuild()
{
	return &g_Build[0];
}
