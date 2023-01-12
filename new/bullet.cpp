//=============================================================================
//
// ���f������ [player.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "model.h"
#include "player.h"
#include "bullet.h"
#include "shadow.h"
#include "light.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_BULLET01		"data/MODEL/apple.obj"			// �ǂݍ��ރ��f����
#define	MODEL_BULLET02		"data/MODEL/pizza.obj"			// �ǂݍ��ރ��f����

#define BULLET_MODEL_TYPE_1 1
#define BULLET_MODEL_TYPE_2 2

#define	VALUE_MOVE			(2.0f)							// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// ��]��

#define BULLET_SHADOW_SIZE	(0.4f)							// �e�̑傫��
#define BULLET_OFFSET_Y		(7.0f)							// �v���C���[�̑��������킹��


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static BULLET				g_Bullet[MAX_BULLET];			// �o���b�g
static PLAYER				g_Player;						// �v���C���[
static CAMERA				g_Camera;						// �J����



int currentBulletModelType = BULLET_MODEL_TYPE_1;



//=============================================================================
// ����������
//=============================================================================
HRESULT InitBullet(void)
{
	//LoadModel(MODEL_Bullet, &g_Bullet[i].model);


	for (int i = 0; i < MAX_BULLET; i++)
	{

		if (currentBulletModelType == BULLET_MODEL_TYPE_1)
		{
			LoadModel(MODEL_BULLET01, &g_Bullet[i].model);
		}
		else if(currentBulletModelType == BULLET_MODEL_TYPE_2)
		{
			LoadModel(MODEL_BULLET02, &g_Bullet[i].model);
		}

		g_Bullet[i].load = true;

		g_Bullet[i].pos = { 0.0f, 0.0f, 0.0f };
		g_Bullet[i].rot = { 0.0f, 0.0f, 0.0f };
		g_Bullet[i].scl = { 10.0f, 10.0f, 10.0f };
		//g_Bullet[i].velocity = { 0.0f, 2.0f, 90.0f };
		g_Bullet[i].velocity = g_Camera.at;
		g_Bullet[i].acceleration = { 0.0f, -9.8f, 0.0f };


		g_Bullet[i].spd = 3.0f;			// �ړ��X�s�[�h�N���A
		g_Bullet[i].size = BULLET_SIZE;	// �����蔻��̑傫��

		g_Bullet[i].use = false;
		g_Bullet[i].move = XMFLOAT3(0.0f, 0.0f, 0.0f);	// �ړ��ʂ�������


		// �����Ńv���C���[�p�̉e���쐬���Ă���
		XMFLOAT3 pos = g_Bullet[i].pos;
		pos.y -= (BULLET_OFFSET_Y - 0.1f);
		g_Bullet[i].shadowIdx = CreateShadow(pos, BULLET_SHADOW_SIZE, BULLET_SHADOW_SIZE);
		//          ��
		//        ���̃����o�[�ϐ������������e��Index�ԍ�
	}

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitBullet(void)
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		// ���f���̉������
		if (g_Bullet[i].load)
		{
			UnloadModel(&g_Bullet[i].model);
			g_Bullet[i].load = false;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBullet(void)
{
	CAMERA* cam = GetCamera();

	if (GetKeyboardTrigger(DIK_1))
	{
		currentBulletModelType = BULLET_MODEL_TYPE_1;
	}
	else if (GetKeyboardTrigger(DIK_2))
	{
		currentBulletModelType = BULLET_MODEL_TYPE_2;
	}

	for (int i = 0; i < MAX_BULLET; i++)
	{
		////	// Key���͂���������ړ���������
		//if (g_Bullet[i].spd > 0.0f)
		//{
		//	//g_Bullet[i].rot.y = g_Bullet[i].dir + cam->rot.y;

		//	// ���͂̂����������փv���C���[���������Ĉړ�������
		//	g_Bullet[i].pos.x -= sinf(g_Bullet[i].rot.y) * g_Bullet[i].spd;
		//	g_Bullet[i].pos.z -= cosf(g_Bullet[i].rot.y) * g_Bullet[i].spd;


		//}

		float dt = 1.0f / 40.0f;
		float speed = 100.0f;


		if (g_Bullet[i].use == TRUE)
		{
			//�����X�s�[�h
			g_Bullet[i].velocity.y += g_Bullet[i].acceleration.y * dt;
			//�ʒu
			g_Bullet[i].pos.x += speed * sinf(g_Camera.at.x) * dt;
			g_Bullet[i].pos.y += g_Bullet[i].velocity.y * dt;
			g_Bullet[i].pos.z += speed * cosf(g_Camera.at.x) * dt;
			//g_Bullet[i].pos.x += g_Bullet[i].velocity.x * dt;
			//g_Bullet[i].pos.y += g_Bullet[i].velocity.y * dt;
			//g_Bullet[i].pos.z += g_Bullet[i].velocity.z * dt;

		}


		//if (g_Bullet[i].use == TRUE)
		//{
		//	float dt = 1.0f / 40.0f;
		//	float bulletSpeed = 300.0f;
		//	XMFLOAT3 bulletVelocity;
		//	XMStoreFloat3(&bulletVelocity, XMVector3Normalize(XMLoadFloat3(&g_Camera.at)) * bulletSpeed);
		//	g_Bullet[i].velocity = bulletVelocity;
		//	g_Bullet[i].pos.x += g_Bullet[i].velocity.x * dt;
		//	g_Bullet[i].pos.y += g_Bullet[i].velocity.y * dt;
		//	g_Bullet[i].pos.z += g_Bullet[i].velocity.z * dt;
		//}

		// �e���v���C���[�̈ʒu�ɍ��킹��
		XMFLOAT3 pos = g_Bullet[i].pos;
		pos.y -= (BULLET_OFFSET_Y - 0.1f);
		SetPositionShadow(g_Bullet[i].shadowIdx, pos);

	}

	//{	// �|�C���g���C�g�̃e�X�g
	//	LIGHT *light = GetLightData(1);
	//	XMFLOAT3 pos = g_Bullet.pos;
	//	pos.y += 20.0f;

	//	light->Position = pos;
	//	light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//	light->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//	light->Type = LIGHT_TYPE_POINT;
	//	light->Enable = true;
	//	SetLightData(1, light);
	//}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBullet(void)
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (g_Bullet[i].use == TRUE)		// ���̃o���b�g���g���Ă���H
		{
			// �J�����O����
			SetCullingMode(CULL_MODE_NONE);

			XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_Bullet[i].scl.x / 4, g_Bullet[i].scl.y / 4, g_Bullet[i].scl.z / 4);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_Bullet[i].rot.x, g_Bullet[i].rot.y + XM_PI, g_Bullet[i].rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_Bullet[i].pos.x, g_Bullet[i].pos.y, g_Bullet[i].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Bullet[i].mtxWorld, mtxWorld);

			//if (g_Bullet[i].use == TRUE)
			//{
			//	XMFLOAT3 bulletVelocity;
			//	XMStoreFloat3(&bulletVelocity, XMVector3Normalize(XMLoadFloat3(&g_Camera.at)) * g_Bullet[i].spd);
			//	g_Bullet[i].velocity = bulletVelocity;
			//}


			// ���f���`��
			DrawModel(&g_Bullet[i].model);

			// �J�����O�ݒ��߂�
			SetCullingMode(CULL_MODE_BACK);
		}

	}
}


//=============================================================================
// �v���C���[�����擾
//=============================================================================
BULLET* GetBullet(void)
{
	return &g_Bullet[0];
}



//=============================================================================
// �o���b�g�̔��ːݒ�
//=============================================================================
void SetBullet(XMFLOAT3 pos, XMFLOAT3 rot)
{
	// �������g�p�̒e�����������甭�˂��Ȃ�( =����ȏ㌂�ĂȂ����Ď� )
	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (g_Bullet[i].use == FALSE)		// ���g�p��Ԃ̃o���b�g��������
		{
			if (currentBulletModelType == BULLET_MODEL_TYPE_1)
			{
				LoadModel(MODEL_BULLET01, &g_Bullet[i].model);
			}
			else if (currentBulletModelType == BULLET_MODEL_TYPE_2)
			{
				LoadModel(MODEL_BULLET02, &g_Bullet[i].model);
			}

			g_Bullet[i].use = TRUE;			// �g�p��Ԃ֕ύX����
			g_Bullet[i].pos = pos;			// ���W���Z�b�g
			g_Bullet[i].rot = rot;			// ���W���Z�b�g

			return;							// 1���Z�b�g�����̂ŏI������
		}
	}
}
