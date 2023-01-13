//=============================================================================
//
// ���C������ [game.cpp]
// Author : �q�Z�C�V��
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
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

int Mode = MODE_TITLE;

int GameMode = GMODE_GAME;

BOOL g_LoadGame = FALSE;					// NewGame

//=============================================================================
// ����������
//=============================================================================
void InitGame(void)
{
	// ���C�g�̏�����
	InitLight();

	//// �^�C�g���̏�����
	InitTitle();

	//// �`���[�g�[���A���̏�����
	InitTutorial();

	//// ���U���g�̏�����
	InitResult();

	// �ǂ̏�����
	InitMesh();

	// �e�̏���������
	InitShadow();

	// �v���C���[�̏�����
	InitPlayer();

	// �G�l�~�[�̏�����
	InitEnemy();

	// �؂𐶂₷
	//InitTree();

	// �e�̏�����
	InitBullet();

	// �r���h�̏�����
	InitBuild();

	// ���C�g��L����
	SetLightEnable(true);

	// �w�ʃ|���S�����J�����O
	SetCullingMode(CULL_MODE_BACK);


	SetMode(Mode);
}

//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{

	// �r���h�̏I������
	UninitBuild();

	// �e�̏I������
	UninitBullet();

	// �؂̏I������
	//UninitTree();

	// �G�l�~�[�̏I������
	UninitEnemy();

	// �v���C���[�̏I������
	UninitPlayer();

	// �e�̏I������
	UninitShadow();

	// �ǂ̏I������
	UninitMeshWall();

	// �n�ʂ̏I������
	UninitMeshField();

	//// ���U���g�̏I������
	UninitResult();

	//// �`���[�g�[���A���̏I������
	UninitTutorial();

	//// �^�C�g���̏I������
	UninitTitle();

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{
	switch (Mode)
	{
	case MODE_TITLE:
		// �^�C�g���̍X�V
		UpdateTitle();
		break;
	case MODE_TUTORIAL:
		// �`���[�g�[���A���̍X�V
		UpdateTutorial();
		break;
	case MODE_GAME:
		// �n�ʏ����̍X�V
		UpdateMeshField();
		// �Ǐ����̍X�V
		UpdateMeshWall();
		// �r���h�̍X�V����
		UpdateBuild();
		// �v���C���[�̍X�V����
		UpdatePlayer();
		// �G�l�~�[�̍X�V����
		UpdateEnemy();
		// �؂̍X�V����
		//UpdateTree();
		// �e�̍X�V����
		UpdateBullet();

		UpdateLight();
		// �e�̍X�V����
		UpdateShadow();
		// �����蔻��
		CheckHit();

		SetCursorPos(SCREEN_CENTER_X, SCREEN_CENTER_Y);

		break;
	case MODE_RESULT:
		// ���U���g�̍X�V
		UpdateResult();
		break;
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame(void)
{
	switch (Mode)
	{
	case MODE_GAME:
		// �n�ʂ̕`�揈��
		//DrawMeshField();
		// �e�̕`�揈��
		DrawShadow();
		// �v���C���[�̕`�揈��
		DrawPlayer();
		// �e�̕`�揈��
		DrawBullet();
		// �G�l�~�[�̕`�揈��
		DrawEnemy();
		DrawBuild();
		// �ǂ̕`�揈��
		//DrawMeshWall();
		// �؂̕`�揈��
		//DrawTree();
		break;
	}


	//---------2D�̕`��------------
	{
		// Z��r�Ȃ�
		SetDepthEnable(false);
		// ���C�e�B���O�𖳌�
		SetLightEnable(false);
		// �^�C�g���̕`�揈��
		switch (Mode)
		{
		case MODE_TITLE:
			// �^�C�g���̕`�揈��
			DrawTitle();
			break;
		case MODE_TUTORIAL:
			// �`���[�g�[���A���̕`�揈��
			DrawTutorial();
			break;
		case MODE_GAME:

			break;
		case MODE_RESULT:
			// ���U���g�̕`�揈��
			DrawResult();

			break;
		}

		// ���C�e�B���O��L����
		SetLightEnable(true);
		// Z��r����
		SetDepthEnable(true);
	}

}



//=============================================================================
// �Z�b�g���[�h�@���@�V�X�e���֌W
//=============================================================================
void SetMode(int mode)
{
	// ���[�h��ς���O�ɑS������������������Ⴄ
	StopSound();			// �܂��Ȃ��~�߂�

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
		// ���C�g�̏�����
		InitLight();

		// �ǂ̏�����
		InitMesh();

		// �e�̏���������
		InitShadow();

		// �v���C���[�̏�����
		InitPlayer();

		// �G�l�~�[�̏�����
		InitEnemy();

		// �؂𐶂₷
		InitTree();

		// �e�̏�����
		InitBullet();

		// �r���h�̏�����
		InitBuild();

		// �J�[�\���̏�����
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
		//	g_LoadGame = FALSE;		// ���[�h��������t���O��Clear����
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
// �Z�b�g���[�h�@���@�Q�[���֌W
//=============================================================================
void SetGameMode(int gamemode)
{
	// ���[�h��ς���O�ɑS������������������Ⴄ
	//StopSound();			// �܂��Ȃ��~�߂�

	GameMode = gamemode;
}


//=============================================================================
// �Q�b�g���[�h�@���@�V�X�e���֌W
//=============================================================================
int GetMode(void)
{
	return Mode;
}

//=============================================================================
// �Q�b�g���[�h�@���@�Q�[���֌W
//=============================================================================
int GetGameMode(void)
{
	return GameMode;
}

//=============================================================================
// �����蔻�菈��
//=============================================================================
void CheckHit(void)
{
	ENEMY* enemy = GetEnemy();		// �G�l�~�[�̃|�C���^�[��������
	PLAYER* player = GetPlayer();	// �v���C���[�̃|�C���^�[��������
	BULLET* bullet = GetBullet();	// �e�̃|�C���^�[��������
	BUILD* build = GetBuild();	// �r���h�̃|�C���^�[��������


	// �G�ƃv���C���[�L����
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//�G�̗L���t���O���`�F�b�N����
		if (enemy[i].use == false)
			continue;

		if (player->use == false)
			break;

		//BC�̓����蔻��
		if (CollisionBC(player->pos, enemy[i].pos, player->size, enemy[i].size))
		{
			// �G�L�����N�^�[�͓|�����
			enemy[i].use = false;
			ReleaseShadow(enemy[i].shadowIdx);

			// �X�R�A�𑫂�

		}
	}

	// �v���C���[�̒e�ƓG
	for (int i = 0; i < MAX_BULLET; i++)
	{
		//�e�̗L���t���O���`�F�b�N����
		if (bullet[i].use == false)
			continue;

		// �G�Ɠ������Ă邩���ׂ�
		for (int j = 0; j < MAX_ENEMY; j++)
		{
			//�G�̗L���t���O���`�F�b�N����
			if (enemy[j].use == false)
				continue;

			//BC�̓����蔻��
			if (CollisionBC(bullet[i].pos, enemy[j].pos, bullet[i].size, enemy[j].size))
			{
				// �����������疢�g�p�ɖ߂�
				bullet[i].use = false;
				ReleaseShadow(bullet[i].shadowIdx);

				// �G�L�����N�^�[�͓|�����
				enemy[j].use = false;
				ReleaseShadow(enemy[j].shadowIdx);

				// �X�R�A�𑫂�
				//AddScore(10);
			}
		}

	}

	// �v���C���[�ƃr���h

	{
		// �G�Ɠ������Ă邩���ׂ�
		for (int i = 0; i < MAX_BUILD; i++)
		{

			//BC�̓����蔻��
			if (CollisionBC(player->pos, build[i].pos, player->size, build[i].size))
			{
				//// ������������߂̈ʒu�ɖ߂�
				player->pos = player->repos;

			}
		}

	}


	// �G�l�~�[���S�����S�������ԑJ��
	int enemy_count = 0;
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy[i].use == false) continue;
		enemy_count++;
	}

	// �G�l�~�[���O�C�H
	if (enemy_count == 0)
	{
		SetFade(FADE_OUT, MODE_RESULT);
	}

}


//=============================================================================
// �ǂ̏�����
//=============================================================================
void InitMesh(void)
{
	// �t�B�[���h�̏�����
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 100, 100, 13.0f, 13.0f);

	// �ǂ̏�����
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f, XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);

	// ��(�����p�̔�����)
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f, XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
}