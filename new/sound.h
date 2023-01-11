//=============================================================================
//
// �T�E���h���� [sound.h]
// Author :  GP11A132 15 ��q�M
// 
//=============================================================================
#pragma once

#include <windows.h>
#include "xaudio2.h"						// �T�E���h�����ŕK�v

//*****************************************************************************
// �T�E���h�t�@�C��
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
// �v���g�^�C�v�錾
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);

