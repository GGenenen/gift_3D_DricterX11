//=============================================================================
//
// �G�l�~�[���f������ [enemy.cpp]
// Author : �q�Z�C�V��
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "build.h"
#include "shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define	VALUE_MOVE			(5.0f)						// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// ��]��

#define BUILD_SHADOW_SIZE	(0.4f)						// �e�̑傫��
#define BUILD_OFFSET_Y		(7.0f)						// �G�l�~�[�̑��������킹��

#define	MODEL_0			"data/MODEL/stoneWall.obj"		// �ǂݍ��ރ��f����
#define	MODEL_1			"data/MODEL/stoneWallCurve.obj"		// �ǂݍ��ރ��f����
#define	MODEL_2			"data/MODEL/ironFenceBorderColumn.obj"		// �ǂݍ��ރ��f����
#define	MODEL_3			"data/MODEL/blockRoundedLarge.obj"		// �ǂݍ��ރ��f����
#define	MODEL_4			"data/MODEL/ironFenceBorderGate.obj"		// �ǂݍ��ރ��f����
#define	MODEL_5			"data/MODEL/ironFenceBorder.obj"		// �ǂݍ��ރ��f����
#define	MODEL_6			"data/MODEL/ironFenceDamaged.obj"		// �ǂݍ��ރ��f����







//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
//static char* g_ModleName[] =
//{
//	"data/MODEL/tate_01.obj"
//};


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static BUILD			g_Build[MAX_BUILD];				// �G�l�~�[




int g_Build_load = 0;

float size = 10.0f;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBuild(void)
{

	for (int i = 0; i < MAX_BUILD; i++)
	{
		g_Build[i].load = true;

	}

	LoadModel(MODEL_0, &g_Build[0].model);
	LoadModel(MODEL_1, &g_Build[1].model);
	LoadModel(MODEL_0, &g_Build[2].model);
	LoadModel(MODEL_1, &g_Build[3].model);
	LoadModel(MODEL_0, &g_Build[4].model);
	LoadModel(MODEL_2, &g_Build[5].model);
	LoadModel(MODEL_2, &g_Build[6].model);
	LoadModel(MODEL_3, &g_Build[7].model);
	LoadModel(MODEL_4, &g_Build[8].model);
	LoadModel(MODEL_5, &g_Build[9].model);
	LoadModel(MODEL_6, &g_Build[18].model);


	g_Build[0].pos = XMFLOAT3(0.0f, 0.0f, 600.0f);
	g_Build[0].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Build[0].scl = XMFLOAT3(size * 12, size, size);

	g_Build[1].pos = XMFLOAT3(600.0f, 0.0f, 600.0f);
	g_Build[1].rot = XMFLOAT3(0.0f, XM_PI , 0.0f);
	g_Build[1].scl = XMFLOAT3(size, size, size);

	g_Build[2].pos = XMFLOAT3(600.0f, 0.0f, 60.0f);
	g_Build[2].rot = XMFLOAT3(0.0f, XM_PI/2, 0.0f);
	g_Build[2].scl = XMFLOAT3(size * 10, size, size);

	g_Build[3].pos = XMFLOAT3(-600.0f, 0.0f, 600.0f);
	g_Build[3].rot = XMFLOAT3(0.0f, XM_PI/2, 0.0f);
	g_Build[3].scl = XMFLOAT3(size, size, size);

	g_Build[4].pos = XMFLOAT3(-690.0f, 0.0f, 60.0f);
	g_Build[4].rot = XMFLOAT3(0.0f, XM_PI / 2, 0.0f);
	g_Build[4].scl = XMFLOAT3(size * 10, size, size);

	g_Build[5].pos = XMFLOAT3(-600.0f, 0.0f, -482.0f);//bc
	g_Build[5].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Build[5].scl = XMFLOAT3(size, size, size);
	g_Build[5].size = 50.0f;

	g_Build[6].pos = XMFLOAT3(600.0f, 0.0f, -400.0f);//bc
	g_Build[6].rot = XMFLOAT3(0.0f, XM_PI, 0.0f);
	g_Build[6].scl = XMFLOAT3(size, size, size);
	g_Build[6].size = 50.0f;

	g_Build[7].pos = XMFLOAT3(0.0f, -100.0f, 0.0f);
	g_Build[7].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Build[7].scl = XMFLOAT3(size*10, size, size*10);

	g_Build[8].pos = XMFLOAT3(400.0f, 0.0f, -400.0f);//door
	g_Build[8].rot = XMFLOAT3(0.0f, XM_PI, 0.0f);
	g_Build[8].scl = XMFLOAT3(size, size, size);

	g_Build[9].pos = XMFLOAT3(500.0f, 0.0f, -400.0f);//bc
	g_Build[9].rot = XMFLOAT3(0.0f, XM_PI, 0.0f);
	g_Build[9].scl = XMFLOAT3(size , size, size );
	g_Build[9].size = 50.0f;

	g_Build[18].pos = XMFLOAT3(-500.0f, 0.0f, -400.0f);
	g_Build[18].rot = XMFLOAT3(0.0f, XM_PI, 0.0f);
	g_Build[18].scl = XMFLOAT3(size, size, size);


	for (int j = 10; j < 18; j++)
	{
		LoadModel(MODEL_5, &g_Build[j].model);
		g_Build[j].pos = XMFLOAT3(1300.0f-j*100.0f, 0.0f, -400.0f);//bc
		g_Build[j].rot = XMFLOAT3(0.0f, XM_PI, 0.0f);
		g_Build[j].scl = XMFLOAT3(size, size, size);
		g_Build[j].size = 50.0f;
	}







	// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
	GetModelDiffuse(&g_Build[0].model, &g_Build[0].diffuse[0]);


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBuild(void)
{

	for (int i = 0; i < MAX_BUILD; i++)
	{
		if (g_Build[i].load)
		{
			UnloadModel(&g_Build[i].model);
			g_Build[i].load = false;
		}
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBuild(void)
{




#ifdef _DEBUG


#endif


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBuild(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_BUILD; i++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Build[i].scl.x, g_Build[i].scl.y, g_Build[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Build[i].rot.x, g_Build[i].rot.y + XM_PI, g_Build[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Build[i].pos.x, g_Build[i].pos.y, g_Build[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Build[i].mtxWorld, mtxWorld);


		// ���f���`��
		DrawModel(&g_Build[i].model);
	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
BUILD* GetBuild()
{
	return &g_Build[0];
}
