//=============================================================================
//
// ライト処理 [light.cpp]
// Author : ヒセイシン
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LIGHT	g_Light[LIGHT_MAX];		//ライトを制御するための配列で現在5(LIGHT_MAX)個分

static FOG		g_Fog;					//霧（フォグ）の制御	

static BOOL		g_FogEnable = FALSE;			//霧 ON/OFF

int Lighting=0;
float LightingF = 0;
bool LightType = false;
//=============================================================================
// 初期化処理
//=============================================================================
void InitLight(void)
{

	//ライト初期化
	for (int i = 0; i < LIGHT_MAX; i++)
	{
		g_Light[i].Position  = XMFLOAT3( 0.0f, 0.0f, 0.0f );
		g_Light[i].Direction = XMFLOAT3( 0.0f, -1.0f, 0.0f );
		g_Light[i].Diffuse   = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
		g_Light[i].Ambient   = XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
		g_Light[i].Attenuation = 150.0f;	// 減衰距離
		g_Light[i].Type = LIGHT_TYPE_NONE;	// ライトのタイプ
		g_Light[i].Enable = false;			// ON / OFF
		SetLight(i, &g_Light[i]);
	}

	// 平行光源の設定（世界を照らす光）
	g_Light[0].Direction = XMFLOAT3( 0.0f, -1.0f, -1.0f );		// 光の向き
	g_Light[0].Diffuse   = XMFLOAT4(0.9f, 0.5f, 0.2f, 1.0f);	// 光の色
	g_Light[0].Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.3f);
	g_Light[0].Type = LIGHT_TYPE_DIRECTIONAL;					// 平行光源
	g_Light[0].Enable = true;									// このライトをON
	SetLight(0, &g_Light[0]);									// これで設定している
	//      ↑        ↑
	//  ライト番号   セットしたい配列のアドレス

		// 平行光源の設定（世界を照らす光）
	g_Light[1].Direction = XMFLOAT3(0.0f, -1.0f, 1.0f);		// 光の向き
	g_Light[1].Diffuse = XMFLOAT4(0.8f, 0.8f, 1.0f, 1.0f);	// 光の色
	g_Light[1].Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.3f);
	g_Light[1].Type = LIGHT_TYPE_DIRECTIONAL;					// 平行光源
	g_Light[1].Enable = true;									// このライトをON
	SetLight(1, &g_Light[1]);									// これで設定している


	//g_Light[1].Position = XMFLOAT3(-50.0f, 60.0f, 50.0f);		// 光のポジション
	//g_Light[1].Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);			// 光の向き
	//g_Light[1].Diffuse = XMFLOAT4(0.2f, 1.0f, 0.2f, 1.0f);		// 光の色
	//g_Light[1].Ambient = XMFLOAT4(0.2f, 1.0f, 0.2f, 1.0f);
	//g_Light[1].Type = LIGHT_TYPE_POINT;							// 点光源
	//g_Light[1].Enable = true;									// このライトをON
	//SetLight(1, &g_Light[1]);									// これで設定している

	//// 点光源の設定
	//g_Light[2].Position = XMFLOAT3(50.0f, 60.0f, 50.0f);		// 光のポジション
	//g_Light[2].Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);			// 光の向き
	//g_Light[2].Diffuse = XMFLOAT4(1.0f, 0.2f, 0.2f, 1.0f);		// 光の色
	//g_Light[2].Type = LIGHT_TYPE_POINT;							// 点光源
	//g_Light[2].Enable = true;									// このライトをON
	//SetLight(2, &g_Light[2]);

	//// 点光源の設定
	//g_Light[3].Position = XMFLOAT3(0.0f, 60.0f, -40.0f);		// 光のポジション
	//g_Light[3].Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);			// 光の向き
	//g_Light[3].Diffuse = XMFLOAT4(0.2f, 0.2f, 1.0f, 1.0f);		// 光の色
	//g_Light[3].Type = LIGHT_TYPE_POINT;							// 点光源
	//g_Light[3].Enable = true;									// このライトをON
	//SetLight(3, &g_Light[3]);

	//// 点光源の設定
	//g_Light[4].Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);			// 光の向き
	//g_Light[4].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);		// 光の色
	//g_Light[4].Type = LIGHT_TYPE_POINT;							// 点光源
	//g_Light[4].Enable = true;									// このライトをON
	//SetLight(4, &g_Light[4]);

	//// 点光源の設定
	//g_Light[5].Position = XMFLOAT3(-100.0f, 60.0f, -50.0f);			// 光の向き
	//g_Light[5].Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);			// 光の向き
	//g_Light[5].Diffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);		// 光の色
	//g_Light[5].Type = LIGHT_TYPE_POINT;							// 点光源
	//g_Light[5].Enable = true;									// このライトをON
	//SetLight(5, &g_Light[5]);

	// フォグの初期化（霧の効果）
	g_Fog.FogStart = 50.0f;									// 視点からこの距離離れるとフォグがかかり始める
	g_Fog.FogEnd   = 400.0f;									// ここまで離れるとフォグの色で見えなくなる
	g_Fog.FogColor = XMFLOAT4{ 0.2f, 0.5f, 0.6f, 0.5f};		// フォグの色
	SetFog(&g_Fog);
	SetFogEnable(g_FogEnable);				// 他の場所もチェックする shadow

}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateLight(void)
{
	if (LightType == true)
	{
		int randtime = rand() % 50;

		if (randtime <= 35)
		{
			Lighting += rand() % 80;
		}
		else if (randtime > 35 && randtime < 40)
		{
			Lighting += rand() % 30 + 50;
		}
		else
		{
			Lighting += rand() % 50 + 50;
		}

		if (Lighting < 5000)
		{
			g_Light[0].Enable = false;
			SetLight(0, &g_Light[0]);

			for (int i = 1; i < 4; i++)
			{
				g_Light[i].Enable = true;
				SetLight(i, &g_Light[i]);
			}
		}
		else if (Lighting >= 5000 && Lighting <= 6000)
		{
			g_Light[0].Enable = true;
			SetLight(0, &g_Light[0]);

			for (int i = 1; i < 4; i++)
			{
				g_Light[i].Enable = false;
				SetLight(i, &g_Light[i]);
			}
		}
		else
		{
			for (int i = 1; i < 4; i++)
			{
				float red = rand() % 200 * 0.01f;
				float green = rand() % 200 * 0.01f;
				float blue = rand() % 200 * 0.01f;

				g_Light[i].Diffuse = XMFLOAT4(red, green, blue, 1.0f);		// 光の色

				SetLight(i, &g_Light[i]);
			}
			Lighting = 0;
		}
	}
	else
	{
		LightingF += 0.02f;
		for (int i = 1; i < 4; i++)
		{
			float red, green, blue;
			switch (i)
			{
			case 1:
				red = sinf(LightingF);
				green = cosf(LightingF);
				blue = sinf(LightingF) * cosf(LightingF);
				break;
			case 2:
				green = sinf(LightingF);
				blue = cosf(LightingF);
				red = sinf(LightingF) * cosf(LightingF);
				break;
			case 3:
				blue = sinf(LightingF);
				red = cosf(LightingF);
				green = sinf(LightingF) * cosf(LightingF);
				break;
			}

			g_Light[i].Diffuse = XMFLOAT4(red, green, blue, 1.0f);		// 光の色

			SetLight(i, &g_Light[i]);
		}
	}

}


//=============================================================================
// ライトの設定
// Typeによってセットするメンバー変数が変わってくる
//=============================================================================
void SetLightData(int index, LIGHT *light)
{
	SetLight(index, light);
}


LIGHT *GetLightData(int index)
{
	return(&g_Light[index]);
}


//=============================================================================
// フォグの設定
//=============================================================================
void SetFogData(FOG *fog)
{
	SetFog(fog);
}


BOOL	GetFogEnable(void)
{
	return(g_FogEnable);
}


