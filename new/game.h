//=============================================================================
//
// メイン処理 [game.h]
//
//=============================================================================
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************


// 補間用のデータ構造体を定義
struct INTERPOLATION_DATA
{
	XMFLOAT3	pos;		// 頂点座標
	XMFLOAT3	rot;		// 回転
	XMFLOAT3	scl;		// 拡大縮小
	float		frame;		// 実行フレーム数 ( dt = 1.0f/frame )
};

enum
{
	MODE_TITLE = 0,			// タイトル画面
	MODE_TUTORIAL,			// ゲーム説明画面
	MODE_GAME,				// ゲーム画面
	MODE_RESULT,			// リザルト画面
	MODE_MAX
};


enum
{
	GMODE_GAME =0,			//一般ゲームモード
	GMODE_SETTING,			//ゲーム設定
	GMODE_MAX
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

void SetMode(int);			// セットモード　→　システム関係
void SetGameMode(int);		// セットモード　→　ゲーム関係
int GetMode(void);			// ゲットモード　→　システム関係
int GetGameMode(void);		// ゲットモード　→　ゲーム関係
void CheckHit(void);		// 当たり判定処理
void InitMesh(void);		// 壁の初期化
