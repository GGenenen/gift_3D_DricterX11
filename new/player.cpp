//=============================================================================
//
// モデル処理 [player.cpp]
// Author : ヒセイシン
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "player.h"
#include "shadow.h"
#include "light.h"
#include "bullet.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_PLAYER		"data/MODEL/player.obj"			// 読み込むモデル名

#define	VALUE_MOVE			(0.9f)							// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// 回転量

#define PLAYER_SHADOW_SIZE	(0.4f)							// 影の大きさ
#define PLAYER_OFFSET_Y		(18.0f)							// プレイヤーの足元をあわせる


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static PLAYER				g_Player;						// プレイヤー
static CAMERA				g_Camera;						// プレイヤー



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	LoadModel(MODEL_PLAYER, &g_Player.model);
	g_Player.load = true;

	g_Player.pos = { -282.0f, PLAYER_OFFSET_Y, -280.0f };
	g_Player.rot = { 0.0f, 0.0f, 0.0f };
	g_Player.scl = { 1.0f, 1.0f, 1.0f };

	g_Player.spd = 0.0f;			// 移動スピードクリア
	g_Player.size = PLAYER_SIZE;	// 当たり判定の大きさ

	// ここでプレイヤー用の影を作成している
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	g_Player.shadowIdx = CreateShadow(pos, PLAYER_SHADOW_SIZE, PLAYER_SHADOW_SIZE);
	//          ↑
	//        このメンバー変数が生成した影のIndex番号

	g_Player.use = true;


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	// モデルの解放処理
	if (g_Player.load)
	{
		UnloadModel(&g_Player.model);
		g_Player.load = false;
	}


}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	//ライトの制御
	{
		LIGHT* PlayerLight = GetLightData(4);
		XMFLOAT3 Lightpos = g_Player.pos;

		Lightpos.x -= sinf(g_Player.rot.y) * 10;
		Lightpos.z -= cosf(g_Player.rot.y) * 10;
		Lightpos.y += 10;
		PlayerLight->Position = Lightpos;

		if (GetKeyboardTrigger(DIK_F))
		{
			PlayerLight->Enable = PlayerLight->Enable ? false : true;
		}
		SetLightData(4, PlayerLight);

	}

	CAMERA* cam = GetCamera();

	g_Player.spd *= 0.9f;

	// 移動処理
	if (GetKeyboardPress(DIK_A))
	{
		g_Player.spd = VALUE_MOVE;
		g_Player.pos.x -= g_Player.spd;
		g_Player.dir = XM_PI / 2;
	}
	if (GetKeyboardPress(DIK_D))
	{
		g_Player.spd = VALUE_MOVE;
		g_Player.pos.x += g_Player.spd;
		g_Player.dir = -XM_PI / 2;
	}
	if (GetKeyboardPress(DIK_W))
	{
		g_Player.spd = VALUE_MOVE;
		g_Player.pos.z += g_Player.spd;
		g_Player.dir = XM_PI;
	}
	if (GetKeyboardPress(DIK_S))
	{
		g_Player.spd = VALUE_MOVE;
		g_Player.pos.z -= g_Player.spd;
		g_Player.dir = 0.0f;
	}

#ifdef _DEBUG
	if (GetKeyboardPress(DIK_R))
	{
		g_Player.pos.z = g_Player.pos.x = 0.0f;
		g_Player.rot.y = g_Player.dir = 0.0f;
		g_Player.spd = 0.0f;
	}
#endif


	//	// Key入力があったら移動処理する
	if (g_Player.spd > 0.0f)
	{
		g_Player.rot.y = g_Player.dir + cam->rot.y;


		// 入力のあった方向へプレイヤーを向かせて移動させる
		g_Player.pos.x -= sinf(g_Player.rot.y) * g_Player.spd;
		g_Player.pos.z -= cosf(g_Player.rot.y) * g_Player.spd;
	}




#ifdef _DEBUG
	if (GetKeyboardPress(DIK_R))
	{
		g_Player.pos.z = g_Player.pos.x = 0.0f;
		g_Player.rot.y = g_Player.dir = 0.0f;
		g_Player.spd = 0.0f;
	}
#endif


	//	// Key入力があったら移動処理する
	if (g_Player.spd > 0.0f)
	{
		g_Player.rot.y = g_Player.dir + cam->rot.y;

		// 入力のあった方向へプレイヤーを向かせて移動させる
		g_Player.pos.x -= sinf(g_Player.rot.y) * g_Player.spd;
		g_Player.pos.z -= cosf(g_Player.rot.y) * g_Player.spd;
	}


	// 弾発射処理
	if (GetKeyboardTrigger(DIK_SPACE))
	{
		g_Player.dir = g_Camera.rot.y;
		SetBullet(g_Player.pos, g_Player.rot);
	}


	// 影もプレイヤーの位置に合わせる
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	SetPositionShadow(g_Player.shadowIdx, pos);


	g_Player.spd *= 0.8f;


	{	// ポイントライトのテスト
		LIGHT *light = GetLightData(1);
		XMFLOAT3 pos = g_Player.pos;
		pos.y += 30.0f;

		light->Position = pos;
		light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Type = LIGHT_TYPE_POINT;
		light->Enable = true;
		SetLightData(1, light);
	}


#ifdef _DEBUG	// デバッグ情報を表示する
	PrintDebugProc("Player:↑ → ↓ ←　Space\n");
	PrintDebugProc("Player:X:%f Y:%f Z:%f\n", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScl = XMMatrixScaling(g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(g_Player.rot.x, g_Player.rot.y + XM_PI, g_Player.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_Player.mtxWorld, mtxWorld);


	// モデル描画
	DrawModel(&g_Player.model);

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// プレイヤー情報を取得
//=============================================================================
PLAYER* GetPlayer(void)
{
	return &g_Player;
}
