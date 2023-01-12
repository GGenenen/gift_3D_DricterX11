//=============================================================================
//
// モデル処理 [player.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "model.h"
#include "player.h"
#include "bullet.h"
#include "shadow.h"
#include "light.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_BULLET01		"data/MODEL/apple.obj"			// 読み込むモデル名
#define	MODEL_BULLET02		"data/MODEL/pizza.obj"			// 読み込むモデル名

#define BULLET_MODEL_TYPE_1 1
#define BULLET_MODEL_TYPE_2 2

#define	VALUE_MOVE			(2.0f)							// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// 回転量

#define BULLET_SHADOW_SIZE	(0.4f)							// 影の大きさ
#define BULLET_OFFSET_Y		(7.0f)							// プレイヤーの足元をあわせる


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static BULLET				g_Bullet[MAX_BULLET];			// バレット
static PLAYER				g_Player;						// プレイヤー
static CAMERA				g_Camera;						// カメラ



int currentBulletModelType = BULLET_MODEL_TYPE_1;



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBullet(void)
{
	//LoadModel(MODEL_Bullet, &g_Bullet[i].model);


	for (int i = 0; i < MAX_BULLET; i++)
	{

		if (currentBulletModelType == BULLET_MODEL_TYPE_1)
		{
			LoadModel(MODEL_BULLET01, &g_Bullet[i].model);
		}
		else if(currentBulletModelType == BULLET_MODEL_TYPE_2)
		{
			LoadModel(MODEL_BULLET02, &g_Bullet[i].model);
		}

		g_Bullet[i].load = true;

		g_Bullet[i].pos = { 0.0f, 0.0f, 0.0f };
		g_Bullet[i].rot = { 0.0f, 0.0f, 0.0f };
		g_Bullet[i].scl = { 10.0f, 10.0f, 10.0f };
		//g_Bullet[i].velocity = { 0.0f, 2.0f, 90.0f };
		g_Bullet[i].velocity = g_Camera.at;
		g_Bullet[i].acceleration = { 0.0f, -9.8f, 0.0f };


		g_Bullet[i].spd = 3.0f;			// 移動スピードクリア
		g_Bullet[i].size = BULLET_SIZE;	// 当たり判定の大きさ

		g_Bullet[i].use = false;
		g_Bullet[i].move = XMFLOAT3(0.0f, 0.0f, 0.0f);	// 移動量を初期化


		// ここでプレイヤー用の影を作成している
		XMFLOAT3 pos = g_Bullet[i].pos;
		pos.y -= (BULLET_OFFSET_Y - 0.1f);
		g_Bullet[i].shadowIdx = CreateShadow(pos, BULLET_SHADOW_SIZE, BULLET_SHADOW_SIZE);
		//          ↑
		//        このメンバー変数が生成した影のIndex番号
	}

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitBullet(void)
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		// モデルの解放処理
		if (g_Bullet[i].load)
		{
			UnloadModel(&g_Bullet[i].model);
			g_Bullet[i].load = false;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBullet(void)
{
	CAMERA* cam = GetCamera();

	if (GetKeyboardTrigger(DIK_1))
	{
		currentBulletModelType = BULLET_MODEL_TYPE_1;
	}
	else if (GetKeyboardTrigger(DIK_2))
	{
		currentBulletModelType = BULLET_MODEL_TYPE_2;
	}

	for (int i = 0; i < MAX_BULLET; i++)
	{
		////	// Key入力があったら移動処理する
		//if (g_Bullet[i].spd > 0.0f)
		//{
		//	//g_Bullet[i].rot.y = g_Bullet[i].dir + cam->rot.y;

		//	// 入力のあった方向へプレイヤーを向かせて移動させる
		//	g_Bullet[i].pos.x -= sinf(g_Bullet[i].rot.y) * g_Bullet[i].spd;
		//	g_Bullet[i].pos.z -= cosf(g_Bullet[i].rot.y) * g_Bullet[i].spd;


		//}

		float dt = 1.0f / 40.0f;
		float speed = 100.0f;


		if (g_Bullet[i].use == TRUE)
		{
			//垂直スピード
			g_Bullet[i].velocity.y += g_Bullet[i].acceleration.y * dt;
			//位置
			g_Bullet[i].pos.x += speed * sinf(g_Camera.at.x) * dt;
			g_Bullet[i].pos.y += g_Bullet[i].velocity.y * dt;
			g_Bullet[i].pos.z += speed * cosf(g_Camera.at.x) * dt;
			//g_Bullet[i].pos.x += g_Bullet[i].velocity.x * dt;
			//g_Bullet[i].pos.y += g_Bullet[i].velocity.y * dt;
			//g_Bullet[i].pos.z += g_Bullet[i].velocity.z * dt;

		}


		//if (g_Bullet[i].use == TRUE)
		//{
		//	float dt = 1.0f / 40.0f;
		//	float bulletSpeed = 300.0f;
		//	XMFLOAT3 bulletVelocity;
		//	XMStoreFloat3(&bulletVelocity, XMVector3Normalize(XMLoadFloat3(&g_Camera.at)) * bulletSpeed);
		//	g_Bullet[i].velocity = bulletVelocity;
		//	g_Bullet[i].pos.x += g_Bullet[i].velocity.x * dt;
		//	g_Bullet[i].pos.y += g_Bullet[i].velocity.y * dt;
		//	g_Bullet[i].pos.z += g_Bullet[i].velocity.z * dt;
		//}

		// 影もプレイヤーの位置に合わせる
		XMFLOAT3 pos = g_Bullet[i].pos;
		pos.y -= (BULLET_OFFSET_Y - 0.1f);
		SetPositionShadow(g_Bullet[i].shadowIdx, pos);

	}

	//{	// ポイントライトのテスト
	//	LIGHT *light = GetLightData(1);
	//	XMFLOAT3 pos = g_Bullet.pos;
	//	pos.y += 20.0f;

	//	light->Position = pos;
	//	light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//	light->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//	light->Type = LIGHT_TYPE_POINT;
	//	light->Enable = true;
	//	SetLightData(1, light);
	//}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBullet(void)
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (g_Bullet[i].use == TRUE)		// このバレットが使われている？
		{
			// カリング無効
			SetCullingMode(CULL_MODE_NONE);

			XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_Bullet[i].scl.x / 4, g_Bullet[i].scl.y / 4, g_Bullet[i].scl.z / 4);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_Bullet[i].rot.x, g_Bullet[i].rot.y + XM_PI, g_Bullet[i].rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_Bullet[i].pos.x, g_Bullet[i].pos.y, g_Bullet[i].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Bullet[i].mtxWorld, mtxWorld);

			//if (g_Bullet[i].use == TRUE)
			//{
			//	XMFLOAT3 bulletVelocity;
			//	XMStoreFloat3(&bulletVelocity, XMVector3Normalize(XMLoadFloat3(&g_Camera.at)) * g_Bullet[i].spd);
			//	g_Bullet[i].velocity = bulletVelocity;
			//}


			// モデル描画
			DrawModel(&g_Bullet[i].model);

			// カリング設定を戻す
			SetCullingMode(CULL_MODE_BACK);
		}

	}
}


//=============================================================================
// プレイヤー情報を取得
//=============================================================================
BULLET* GetBullet(void)
{
	return &g_Bullet[0];
}



//=============================================================================
// バレットの発射設定
//=============================================================================
void SetBullet(XMFLOAT3 pos, XMFLOAT3 rot)
{
	// もし未使用の弾が無かったら発射しない( =これ以上撃てないって事 )
	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (g_Bullet[i].use == FALSE)		// 未使用状態のバレットを見つける
		{
			if (currentBulletModelType == BULLET_MODEL_TYPE_1)
			{
				LoadModel(MODEL_BULLET01, &g_Bullet[i].model);
			}
			else if (currentBulletModelType == BULLET_MODEL_TYPE_2)
			{
				LoadModel(MODEL_BULLET02, &g_Bullet[i].model);
			}

			g_Bullet[i].use = TRUE;			// 使用状態へ変更する
			g_Bullet[i].pos = pos;			// 座標をセット
			g_Bullet[i].rot = rot;			// 座標をセット

			return;							// 1発セットしたので終了する
		}
	}
}
