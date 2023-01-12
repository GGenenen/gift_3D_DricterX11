//=============================================================================
//
// ���f������ [player.cpp]
// Author : �q�Z�C�V��
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "player.h"
#include "shadow.h"
#include "light.h"
#include "bullet.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_PLAYER		"data/MODEL/player.obj"			// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(0.9f)							// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// ��]��

#define PLAYER_SHADOW_SIZE	(0.4f)							// �e�̑傫��
#define PLAYER_OFFSET_Y		(18.0f)							// �v���C���[�̑��������킹��


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static PLAYER				g_Player;						// �v���C���[
static CAMERA				g_Camera;						// �v���C���[



//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	LoadModel(MODEL_PLAYER, &g_Player.model);
	g_Player.load = true;

	g_Player.pos = { -282.0f, PLAYER_OFFSET_Y, -280.0f };
	g_Player.rot = { 0.0f, 0.0f, 0.0f };
	g_Player.scl = { 1.0f, 1.0f, 1.0f };

	g_Player.spd = 0.0f;			// �ړ��X�s�[�h�N���A
	g_Player.size = PLAYER_SIZE;	// �����蔻��̑傫��

	// �����Ńv���C���[�p�̉e���쐬���Ă���
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	g_Player.shadowIdx = CreateShadow(pos, PLAYER_SHADOW_SIZE, PLAYER_SHADOW_SIZE);
	//          ��
	//        ���̃����o�[�ϐ������������e��Index�ԍ�

	g_Player.use = true;


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	// ���f���̉������
	if (g_Player.load)
	{
		UnloadModel(&g_Player.model);
		g_Player.load = false;
	}


}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	//���C�g�̐���
	{
		LIGHT* PlayerLight = GetLightData(4);
		XMFLOAT3 Lightpos = g_Player.pos;

		Lightpos.x -= sinf(g_Player.rot.y) * 10;
		Lightpos.z -= cosf(g_Player.rot.y) * 10;
		Lightpos.y += 10;
		PlayerLight->Position = Lightpos;

		if (GetKeyboardTrigger(DIK_F))
		{
			PlayerLight->Enable = PlayerLight->Enable ? false : true;
		}
		SetLightData(4, PlayerLight);

	}

	CAMERA* cam = GetCamera();

	g_Player.spd *= 0.9f;

	// �ړ�����
	if (GetKeyboardPress(DIK_A))
	{
		g_Player.spd = VALUE_MOVE;
		g_Player.pos.x -= g_Player.spd;
		g_Player.dir = XM_PI / 2;
	}
	if (GetKeyboardPress(DIK_D))
	{
		g_Player.spd = VALUE_MOVE;
		g_Player.pos.x += g_Player.spd;
		g_Player.dir = -XM_PI / 2;
	}
	if (GetKeyboardPress(DIK_W))
	{
		g_Player.spd = VALUE_MOVE;
		g_Player.pos.z += g_Player.spd;
		g_Player.dir = XM_PI;
	}
	if (GetKeyboardPress(DIK_S))
	{
		g_Player.spd = VALUE_MOVE;
		g_Player.pos.z -= g_Player.spd;
		g_Player.dir = 0.0f;
	}

#ifdef _DEBUG
	if (GetKeyboardPress(DIK_R))
	{
		g_Player.pos.z = g_Player.pos.x = 0.0f;
		g_Player.rot.y = g_Player.dir = 0.0f;
		g_Player.spd = 0.0f;
	}
#endif


	//	// Key���͂���������ړ���������
	if (g_Player.spd > 0.0f)
	{
		g_Player.rot.y = g_Player.dir + cam->rot.y;


		// ���͂̂����������փv���C���[���������Ĉړ�������
		g_Player.pos.x -= sinf(g_Player.rot.y) * g_Player.spd;
		g_Player.pos.z -= cosf(g_Player.rot.y) * g_Player.spd;
	}




#ifdef _DEBUG
	if (GetKeyboardPress(DIK_R))
	{
		g_Player.pos.z = g_Player.pos.x = 0.0f;
		g_Player.rot.y = g_Player.dir = 0.0f;
		g_Player.spd = 0.0f;
	}
#endif


	//	// Key���͂���������ړ���������
	if (g_Player.spd > 0.0f)
	{
		g_Player.rot.y = g_Player.dir + cam->rot.y;

		// ���͂̂����������փv���C���[���������Ĉړ�������
		g_Player.pos.x -= sinf(g_Player.rot.y) * g_Player.spd;
		g_Player.pos.z -= cosf(g_Player.rot.y) * g_Player.spd;
	}


	// �e���ˏ���
	if (GetKeyboardTrigger(DIK_SPACE))
	{
		g_Player.dir = g_Camera.rot.y;
		SetBullet(g_Player.pos, g_Player.rot);
	}


	// �e���v���C���[�̈ʒu�ɍ��킹��
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	SetPositionShadow(g_Player.shadowIdx, pos);


	g_Player.spd *= 0.8f;


	{	// �|�C���g���C�g�̃e�X�g
		LIGHT *light = GetLightData(1);
		XMFLOAT3 pos = g_Player.pos;
		pos.y += 30.0f;

		light->Position = pos;
		light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Type = LIGHT_TYPE_POINT;
		light->Enable = true;
		SetLightData(1, light);
	}


#ifdef _DEBUG	// �f�o�b�O����\������
	PrintDebugProc("Player:�� �� �� ���@Space\n");
	PrintDebugProc("Player:X:%f Y:%f Z:%f\n", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(g_Player.rot.x, g_Player.rot.y + XM_PI, g_Player.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_Player.mtxWorld, mtxWorld);


	// ���f���`��
	DrawModel(&g_Player.model);

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// �v���C���[�����擾
//=============================================================================
PLAYER* GetPlayer(void)
{
	return &g_Player;
}
