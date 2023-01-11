//=============================================================================
//
// メイン処理 [game.h]
//
//=============================================================================
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************



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
