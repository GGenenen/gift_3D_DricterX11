//=============================================================================
//
// モデル処理 [Bullet.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_BULLET		(120)					// プレイヤーの数
#define	BULLET_SIZE		(3.0f)				// 当たり判定の大きさ



//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct BULLET
{
	XMFLOAT3		pos;		// ポリゴンの位置
	XMFLOAT3		rot;		// ポリゴンの向き(回転)
	XMFLOAT3		scl;		// ポリゴンの大きさ(スケール)

	XMFLOAT3		velocity;
	XMFLOAT3		acceleration;


	XMFLOAT4X4		mtxWorld;	// ワールドマトリックス

	bool			load;
	DX11_MODEL		model;		// モデル情報

	float			spd;		// 移動スピード
	float			dir;		// 向き
	float			size;		// 当たり判定の大きさ
	int				shadowIdx;	// 影のIndex
	bool			use;
	XMFLOAT3			move;				// バレットの移動量


};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

BULLET* GetBullet(void);
void SetBullet(XMFLOAT3 pos, XMFLOAT3 rot);


