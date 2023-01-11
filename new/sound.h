//=============================================================================
//
// サウンド処理 [sound.h]
// Author :  GP11A132 15 趙子勲
// 
//=============================================================================
#pragma once

#include <windows.h>
#include "xaudio2.h"						// サウンド処理で必要

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
enum 
{
	SOUND_LABEL_BGM_0,	// BGM0
	SOUND_LABEL_BGM_1,	// BGM1
	SOUND_LABEL_BGM_2,	// BGM2
	SOUND_LABEL_BGM_3,	// BGM3
	SOUND_LABEL_BGM_4,	// BGM4
	SOUND_LABEL_BGM_5,	// BGM5
	SOUND_LABEL_BGM_6,	// BGM6

	SOUND_LABEL_SE_button,		// 
	SOUND_LABEL_SE_jump,		// 
	SOUND_LABEL_SE_hit01,		// 
	SOUND_LABEL_SE_hit02,		// 

	SOUND_LABEL_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);

