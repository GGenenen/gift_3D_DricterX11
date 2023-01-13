//=============================================================================
//
// メイン処理 [game.cpp]
// Author : ヒセイシン
//
//=============================================================================
//------------------SYSTEM---------------------------
//#include "input.h"
#include "main.h"
#include "model.h"
#include "renderer.h"

//-------------------GAME---------------------------
#include "build.h"
#include "camera.h"
#include "collision.h"
#include "enemy.h"
#include "fade.h"
#include "game.h"
#include "light.h"
#include "meshfield.h"
#include "meshwall.h"
#include "player.h"
#include "result.h"
#include "shadow.h"
#include "sound.h"
#include "title.h"
#include "tree.h"
#include "tutorial.h"
#include "bullet.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// グローバル変数:
//*****************************************************************************

int Mode = MODE_TITLE;

int GameMode = GMODE_GAME;

BOOL g_LoadGame = FALSE;					// NewGame

//=============================================================================
// 初期化処理
//=============================================================================
void InitGame(void)
{
	// ライトの初期化
	InitLight();

	//// タイトルの初期化
	InitTitle();

	//// チュートーリアルの初期化
	InitTutorial();

	//// リザルトの初期化
	InitResult();

	// 壁の初期化
	InitMesh();

	// 影の初期化処理
	InitShadow();

	// プレイヤーの初期化
	InitPlayer();

	// エネミーの初期化
	InitEnemy();

	// 木を生やす
	//InitTree();

	// 弾の初期化
	InitBullet();

	// ビルドの初期化
	InitBuild();

	// ライトを有効化
	SetLightEnable(true);

	// 背面ポリゴンをカリング
	SetCullingMode(CULL_MODE_BACK);


	SetMode(Mode);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{

	// ビルドの終了処理
	UninitBuild();

	// 弾の終了処理
	UninitBullet();

	// 木の終了処理
	//UninitTree();

	// エネミーの終了処理
	UninitEnemy();

	// プレイヤーの終了処理
	UninitPlayer();

	// 影の終了処理
	UninitShadow();

	// 壁の終了処理
	UninitMeshWall();

	// 地面の終了処理
	UninitMeshField();

	//// リザルトの終了処理
	UninitResult();

	//// チュートーリアルの終了処理
	UninitTutorial();

	//// タイトルの終了処理
	UninitTitle();

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{
	switch (Mode)
	{
	case MODE_TITLE:
		// タイトルの更新
		UpdateTitle();
		break;
	case MODE_TUTORIAL:
		// チュートーリアルの更新
		UpdateTutorial();
		break;
	case MODE_GAME:
		// 地面処理の更新
		UpdateMeshField();
		// 壁処理の更新
		UpdateMeshWall();
		// ビルドの更新処理
		UpdateBuild();
		// プレイヤーの更新処理
		UpdatePlayer();
		// エネミーの更新処理
		UpdateEnemy();
		// 木の更新処理
		//UpdateTree();
		// 弾の更新処理
		UpdateBullet();

		UpdateLight();
		// 影の更新処理
		UpdateShadow();
		// 当たり判定
		CheckHit();

		SetCursorPos(SCREEN_CENTER_X, SCREEN_CENTER_Y);

		break;
	case MODE_RESULT:
		// リザルトの更新
		UpdateResult();
		break;
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame(void)
{
	switch (Mode)
	{
	case MODE_GAME:
		// 地面の描画処理
		//DrawMeshField();
		// 影の描画処理
		DrawShadow();
		// プレイヤーの描画処理
		DrawPlayer();
		// 弾の描画処理
		DrawBullet();
		// エネミーの描画処理
		DrawEnemy();
		DrawBuild();
		// 壁の描画処理
		//DrawMeshWall();
		// 木の描画処理
		//DrawTree();
		break;
	}


	//---------2Dの描画------------
	{
		// Z比較なし
		SetDepthEnable(false);
		// ライティングを無効
		SetLightEnable(false);
		// タイトルの描画処理
		switch (Mode)
		{
		case MODE_TITLE:
			// タイトルの描画処理
			DrawTitle();
			break;
		case MODE_TUTORIAL:
			// チュートーリアルの描画処理
			DrawTutorial();
			break;
		case MODE_GAME:

			break;
		case MODE_RESULT:
			// リザルトの描画処理
			DrawResult();

			break;
		}

		// ライティングを有効に
		SetLightEnable(true);
		// Z比較あり
		SetDepthEnable(true);
	}

}



//=============================================================================
// セットモード　→　システム関係
//=============================================================================
void SetMode(int mode)
{
	// モードを変える前に全部メモリを解放しちゃう
	StopSound();			// まず曲を止める

	UninitGame();

	Mode = mode;

	switch (Mode)
	{
	case MODE_TITLE:
		InitTitle();
		PlaySound(SOUND_LABEL_BGM_0);
		break;
	case MODE_TUTORIAL:
		InitTutorial();
		PlaySound(SOUND_LABEL_BGM_0);
		break;
	case MODE_GAME:
		// ライトの初期化
		InitLight();

		// 壁の初期化
		InitMesh();

		// 影の初期化処理
		InitShadow();

		// プレイヤーの初期化
		InitPlayer();

		// エネミーの初期化
		InitEnemy();

		// 木を生やす
		InitTree();

		// 弾の初期化
		InitBullet();

		// ビルドの初期化
		InitBuild();

		// カーソルの初期化
		ShowCursor(FALSE);


		//switch (GameMode)
		//{
		//case GMODE_GAME:
		//	break;
		//}
		// 
		//if (g_LoadGame == TRUE)
		//{
		//	LoadData();
		//	g_LoadGame = FALSE;		// ロードしたからフラグをClearする
		//}

		PlaySound(SOUND_LABEL_BGM_1);
		break;
	case MODE_RESULT:
		InitResult();
		PlaySound(SOUND_LABEL_BGM_0);
		break;
	}

}

//=============================================================================
// セットモード　→　ゲーム関係
//=============================================================================
void SetGameMode(int gamemode)
{
	// モードを変える前に全部メモリを解放しちゃう
	//StopSound();			// まず曲を止める

	GameMode = gamemode;
}


//=============================================================================
// ゲットモード　→　システム関係
//=============================================================================
int GetMode(void)
{
	return Mode;
}

//=============================================================================
// ゲットモード　→　ゲーム関係
//=============================================================================
int GetGameMode(void)
{
	return GameMode;
}

//=============================================================================
// 当たり判定処理
//=============================================================================
void CheckHit(void)
{
	ENEMY* enemy = GetEnemy();		// エネミーのポインターを初期化
	PLAYER* player = GetPlayer();	// プレイヤーのポインターを初期化
	BULLET* bullet = GetBullet();	// 弾のポインターを初期化
	BUILD* build = GetBuild();	// ビルドのポインターを初期化


	// 敵とプレイヤーキャラ
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//敵の有効フラグをチェックする
		if (enemy[i].use == false)
			continue;

		if (player->use == false)
			break;

		//BCの当たり判定
		if (CollisionBC(player->pos, enemy[i].pos, player->size, enemy[i].size))
		{
			// 敵キャラクターは倒される
			enemy[i].use = false;
			ReleaseShadow(enemy[i].shadowIdx);

			// スコアを足す

		}
	}

	// プレイヤーの弾と敵
	for (int i = 0; i < MAX_BULLET; i++)
	{
		//弾の有効フラグをチェックする
		if (bullet[i].use == false)
			continue;

		// 敵と当たってるか調べる
		for (int j = 0; j < MAX_ENEMY; j++)
		{
			//敵の有効フラグをチェックする
			if (enemy[j].use == false)
				continue;

			//BCの当たり判定
			if (CollisionBC(bullet[i].pos, enemy[j].pos, bullet[i].size, enemy[j].size))
			{
				// 当たったから未使用に戻す
				bullet[i].use = false;
				ReleaseShadow(bullet[i].shadowIdx);

				// 敵キャラクターは倒される
				enemy[j].use = false;
				ReleaseShadow(enemy[j].shadowIdx);

				// スコアを足す
				//AddScore(10);
			}
		}

	}

	// プレイヤーとビルド

	{
		// 敵と当たってるか調べる
		for (int i = 0; i < MAX_BUILD; i++)
		{

			//BCの当たり判定
			if (CollisionBC(player->pos, build[i].pos, player->size, build[i].size))
			{
				//// 当たったから戻の位置に戻す
				player->pos = player->repos;

			}
		}

	}


	// エネミーが全部死亡したら状態遷移
	int enemy_count = 0;
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy[i].use == false) continue;
		enemy_count++;
	}

	// エネミーが０匹？
	if (enemy_count == 0)
	{
		SetFade(FADE_OUT, MODE_RESULT);
	}

}


//=============================================================================
// 壁の初期化
//=============================================================================
void InitMesh(void)
{
	// フィールドの初期化
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 100, 100, 13.0f, 13.0f);

	// 壁の初期化
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f, XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);

	// 壁(裏側用の半透明)
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f, XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
}