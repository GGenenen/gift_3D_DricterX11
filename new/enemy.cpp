//=============================================================================
//
// モデル処理 [enemy.cpp]
//Author : ヒセイシン
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "enemy.h"
#include "player.h"
#include "stdlib.h"
#include "shadow.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_ENEMY		"data/MODEL/test2.obj"			// 読み込むモデル名

#define	VALUE_MOVE			(2.0f)							// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// 回転量

#define ENEMY_SHADOW_SIZE	(0.4f)							// 影の大きさ
#define ENEMY_OFFSET_Y		(0.0f)							// エネミーの足元をあわせる


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ENEMY				g_Enemy[MAX_ENEMY];						// エネミー

static INTERPOLATION_DATA g_MoveTbl0[] = {
	//座標									回転率							拡大率					時間
	{ XMFLOAT3(0.0f,  0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.2f, 0.2f, 0.2f),	20 },
	{ XMFLOAT3(0.0f,  10.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.2f, 0.2f, 0.2f),	20 },
};


static INTERPOLATION_DATA g_MoveTbl1[] = {
	//座標									回転率							拡大率						時間
	{ XMFLOAT3(100.0f,   0.0f, 0.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.2f, 0.2f, 0.2f),	120 },
	{ XMFLOAT3(0.0f,   0.0f, 100.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.2f, 0.2f, 0.2f),	120 },
};


static INTERPOLATION_DATA g_MoveTbl2[] = {
	//座標									回転率							拡大率						時間
	{ XMFLOAT3(200.0f, 0.0f, 100.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3(100.0f, 0.0f, 200.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
};

static INTERPOLATION_DATA* g_MoveTblAdr[] =
{
	g_MoveTbl0,
	g_MoveTbl1,
	g_MoveTbl2,

};


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	g_Enemy[0].pos = { 100.0f, ENEMY_OFFSET_Y, 0.0f };
	g_Enemy[1].pos = { 100.0f, ENEMY_OFFSET_Y, 100.0f };
	g_Enemy[2].pos = { 0.0f, ENEMY_OFFSET_Y, 100.0f };

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		LoadModel(MODEL_ENEMY, &g_Enemy[i].model);
		g_Enemy[i].load = true;
		g_Enemy[i].rot = { 0.0f, 0.0f, 0.0f };
		g_Enemy[i].scl = { 0.2f, 0.2f, 0.2f };

		g_Enemy[i].spd = 0.0f;			// 移動スピードクリア
		g_Enemy[i].size = ENEMY_SIZE;	// 当たり判定の大きさ

		g_Enemy[i].use = true;

		GetModelDiffuse(&g_Enemy[i].model, &g_Enemy[i].diffuse[i]);

		// ここでプレイヤー用の影を作成している
		XMFLOAT3 pos = g_Enemy[i].pos;
		pos.y -= (ENEMY_OFFSET_Y - 0.1f);
		g_Enemy[i].shadowIdx = CreateShadow(pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);
		//          ↑
		//        このメンバー変数が生成した影のIndex番号
	}

	// 0番だけ線形補間で動かしてみる
	g_Enemy[0].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[0].tblNo = 0;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[0].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	// 1番だけ線形補間で動かしてみる
	g_Enemy[1].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[1].tblNo = 1;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[1].tblMax = sizeof(g_MoveTbl1) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	// 2番だけ線形補間で動かしてみる
	g_Enemy[2].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[2].tblNo = 2;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[2].tblMax = 0;	// 再生するアニメデータのレコード数をセット

	

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	// モデルの解放処理
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].load)
		{
			ReleaseShadow(g_Enemy[i].shadowIdx);
			UnloadModel(&g_Enemy[i].model);
			g_Enemy[i].load = false;
		}
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	PLAYER* player = GetPlayer();

	for (int i = 0 ;i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == true)		// このエネミーが使われている？
		{								// Yes

			if (g_Enemy[i].tblMax > 0)	// 線形補間を実行する？
			{	// 線形補間の処理
				int nowNo = (int)g_Enemy[i].time;			// 整数分であるテーブル番号を取り出している
				int maxNo = g_Enemy[i].tblMax;				// 登録テーブル数を数えている
				int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
				INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Enemy[i].tblNo];	// 行動テーブルのアドレスを取得

				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換
				XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTORへ変換
				XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTORへ変換

				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している
				XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ回転量を計算している
				XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ拡大率を計算している

				float nowTime = g_Enemy[i].time - nowNo;	// 時間部分である少数を取り出している

				Pos *= nowTime;								// 現在の移動量を計算している
				Rot *= nowTime;								// 現在の回転量を計算している
				Scl *= nowTime;								// 現在の拡大率を計算している

				// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
				XMStoreFloat3(&g_Enemy[i].pos, nowPos + Pos);

				// 計算して求めた回転量を現在の移動テーブルに足している
				XMStoreFloat3(&g_Enemy[i].rot, nowRot + Rot);

				// 計算して求めた拡大率を現在の移動テーブルに足している
				XMStoreFloat3(&g_Enemy[i].scl, nowScl + Scl);
				// frameを使て時間経過処理をする
				g_Enemy[i].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている
				if ((int)g_Enemy[i].time >= maxNo)			// 登録テーブル最後まで移動したか？
				{
					g_Enemy[i].time -= maxNo;				// ０番目にリセットしつつも小数部分を引き継いでいる
				}

			}
			else
			{
				XMFLOAT3 ep = g_Enemy[i].pos;
				XMFLOAT3 pp = player->pos;
				ep.y = 0;
				pp.y = 0;

				XMVECTOR epos = XMLoadFloat3(&ep);
				XMVECTOR vec = XMLoadFloat3(&pp) - epos;

				epos += vec * 0.01f;
				XMStoreFloat3(&g_Enemy[i].pos, epos);


				/*XMVECTOR epos = XMLoadFloat3(&g_Enemy[i].pos);

				XMVECTOR vec = XMLoadFloat3(&player->pos) - epos;

				epos += vec * 0.01f;
				XMStoreFloat3(&g_Enemy[i].pos, epos);*/
			}


			// 影もプレイヤーの位置に合わせる
			XMFLOAT3 pos = g_Enemy[i].pos;
			pos.y = (ENEMY_OFFSET_Y + 0.1f);
			SetPositionShadow(g_Enemy[i].shadowIdx, pos);
		}
	}



#ifdef _DEBUG


	if (GetKeyboardTrigger(DIK_L))
	{
		// モデルの色を元に戻している
		for (int j = 0; j < g_Enemy[2].model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_Enemy[2].model, j, g_Enemy[2].diffuse[j]);
		}
	}
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;


	for(int i=0;i< MAX_ENEMY;i++)
	{
		if (g_Enemy[i].use == false) continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);

		// モデル描画
		DrawModel(&g_Enemy[i].model);
	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// エネミー情報を取得
//=============================================================================
ENEMY* GetEnemy(void)
{
	return &g_Enemy[0];
}

