//=============================================================================
//
// ���f������ [enemy.cpp]
//Author : �q�Z�C�V��
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "enemy.h"
#include "player.h"
#include "stdlib.h"
#include "shadow.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_ENEMY		"data/MODEL/test2.obj"			// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(2.0f)							// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// ��]��

#define ENEMY_SHADOW_SIZE	(0.4f)							// �e�̑傫��
#define ENEMY_OFFSET_Y		(0.0f)							// �G�l�~�[�̑��������킹��


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ENEMY				g_Enemy[MAX_ENEMY];						// �G�l�~�[

static INTERPOLATION_DATA g_MoveTbl0[] = {
	//���W									��]��							�g�嗦					����
	{ XMFLOAT3(0.0f,  0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.2f, 0.2f, 0.2f),	20 },
	{ XMFLOAT3(0.0f,  10.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.2f, 0.2f, 0.2f),	20 },
};


static INTERPOLATION_DATA g_MoveTbl1[] = {
	//���W									��]��							�g�嗦						����
	{ XMFLOAT3(100.0f,   0.0f, 0.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.2f, 0.2f, 0.2f),	120 },
	{ XMFLOAT3(0.0f,   0.0f, 100.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.2f, 0.2f, 0.2f),	120 },
};


static INTERPOLATION_DATA g_MoveTbl2[] = {
	//���W									��]��							�g�嗦						����
	{ XMFLOAT3(200.0f, 0.0f, 100.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3(100.0f, 0.0f, 200.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
};

static INTERPOLATION_DATA* g_MoveTblAdr[] =
{
	g_MoveTbl0,
	g_MoveTbl1,
	g_MoveTbl2,

};


//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
	g_Enemy[0].pos = { 100.0f, ENEMY_OFFSET_Y, 0.0f };
	g_Enemy[1].pos = { 100.0f, ENEMY_OFFSET_Y, 100.0f };
	g_Enemy[2].pos = { 0.0f, ENEMY_OFFSET_Y, 100.0f };

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		LoadModel(MODEL_ENEMY, &g_Enemy[i].model);
		g_Enemy[i].load = true;
		g_Enemy[i].rot = { 0.0f, 0.0f, 0.0f };
		g_Enemy[i].scl = { 0.2f, 0.2f, 0.2f };

		g_Enemy[i].spd = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Enemy[i].size = ENEMY_SIZE;	// �����蔻��̑傫��

		g_Enemy[i].use = true;

		GetModelDiffuse(&g_Enemy[i].model, &g_Enemy[i].diffuse[i]);

		// �����Ńv���C���[�p�̉e���쐬���Ă���
		XMFLOAT3 pos = g_Enemy[i].pos;
		pos.y -= (ENEMY_OFFSET_Y - 0.1f);
		g_Enemy[i].shadowIdx = CreateShadow(pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);
		//          ��
		//        ���̃����o�[�ϐ������������e��Index�ԍ�
	}

	// 0�Ԃ������`��Ԃœ������Ă݂�
	g_Enemy[0].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[0].tblNo = 0;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[0].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 1�Ԃ������`��Ԃœ������Ă݂�
	g_Enemy[1].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[1].tblNo = 1;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[1].tblMax = sizeof(g_MoveTbl1) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 2�Ԃ������`��Ԃœ������Ă݂�
	g_Enemy[2].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[2].tblNo = 2;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[2].tblMax = 0;	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	// ���f���̉������
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].load)
		{
			ReleaseShadow(g_Enemy[i].shadowIdx);
			UnloadModel(&g_Enemy[i].model);
			g_Enemy[i].load = false;
		}
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	PLAYER* player = GetPlayer();

	for (int i = 0 ;i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == true)		// ���̃G�l�~�[���g���Ă���H
		{								// Yes

			if (g_Enemy[i].tblMax > 0)	// ���`��Ԃ����s����H
			{	// ���`��Ԃ̏���
				int nowNo = (int)g_Enemy[i].time;			// �������ł���e�[�u���ԍ������o���Ă���
				int maxNo = g_Enemy[i].tblMax;				// �o�^�e�[�u�����𐔂��Ă���
				int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
				INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Enemy[i].tblNo];	// �s���e�[�u���̃A�h���X���擾

				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
				XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR�֕ϊ�
				XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTOR�֕ϊ�

				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
				XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ��]�ʂ��v�Z���Ă���
				XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ�g�嗦���v�Z���Ă���

				float nowTime = g_Enemy[i].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

				Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
				Rot *= nowTime;								// ���݂̉�]�ʂ��v�Z���Ă���
				Scl *= nowTime;								// ���݂̊g�嗦���v�Z���Ă���

				// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
				XMStoreFloat3(&g_Enemy[i].pos, nowPos + Pos);

				// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
				XMStoreFloat3(&g_Enemy[i].rot, nowRot + Rot);

				// �v�Z���ċ��߂��g�嗦�����݂̈ړ��e�[�u���ɑ����Ă���
				XMStoreFloat3(&g_Enemy[i].scl, nowScl + Scl);
				// frame���g�Ď��Ԍo�ߏ���������
				g_Enemy[i].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���
				if ((int)g_Enemy[i].time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
				{
					g_Enemy[i].time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���
				}

			}
			else
			{
				XMFLOAT3 ep = g_Enemy[i].pos;
				XMFLOAT3 pp = player->pos;
				ep.y = 0;
				pp.y = 0;

				XMVECTOR epos = XMLoadFloat3(&ep);
				XMVECTOR vec = XMLoadFloat3(&pp) - epos;

				epos += vec * 0.01f;
				XMStoreFloat3(&g_Enemy[i].pos, epos);


				/*XMVECTOR epos = XMLoadFloat3(&g_Enemy[i].pos);

				XMVECTOR vec = XMLoadFloat3(&player->pos) - epos;

				epos += vec * 0.01f;
				XMStoreFloat3(&g_Enemy[i].pos, epos);*/
			}


			// �e���v���C���[�̈ʒu�ɍ��킹��
			XMFLOAT3 pos = g_Enemy[i].pos;
			pos.y = (ENEMY_OFFSET_Y + 0.1f);
			SetPositionShadow(g_Enemy[i].shadowIdx, pos);
		}
	}



#ifdef _DEBUG


	if (GetKeyboardTrigger(DIK_L))
	{
		// ���f���̐F�����ɖ߂��Ă���
		for (int j = 0; j < g_Enemy[2].model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_Enemy[2].model, j, g_Enemy[2].diffuse[j]);
		}
	}
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;


	for(int i=0;i< MAX_ENEMY;i++)
	{
		if (g_Enemy[i].use == false) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);

		// ���f���`��
		DrawModel(&g_Enemy[i].model);
	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// �G�l�~�[�����擾
//=============================================================================
ENEMY* GetEnemy(void)
{
	return &g_Enemy[0];
}

