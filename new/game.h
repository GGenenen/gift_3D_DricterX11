//=============================================================================
//
// ���C������ [game.h]
//
//=============================================================================
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************


// ��ԗp�̃f�[�^�\���̂��`
struct INTERPOLATION_DATA
{
	XMFLOAT3	pos;		// ���_���W
	XMFLOAT3	rot;		// ��]
	XMFLOAT3	scl;		// �g��k��
	float		frame;		// ���s�t���[���� ( dt = 1.0f/frame )
};

enum
{
	MODE_TITLE = 0,			// �^�C�g�����
	MODE_TUTORIAL,			// �Q�[���������
	MODE_GAME,				// �Q�[�����
	MODE_RESULT,			// ���U���g���
	MODE_MAX
};


enum
{
	GMODE_GAME =0,			//��ʃQ�[�����[�h
	GMODE_SETTING,			//�Q�[���ݒ�
	GMODE_MAX
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

void SetMode(int);			// �Z�b�g���[�h�@���@�V�X�e���֌W
void SetGameMode(int);		// �Z�b�g���[�h�@���@�Q�[���֌W
int GetMode(void);			// �Q�b�g���[�h�@���@�V�X�e���֌W
int GetGameMode(void);		// �Q�b�g���[�h�@���@�Q�[���֌W
void CheckHit(void);		// �����蔻�菈��
void InitMesh(void);		// �ǂ̏�����
