//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : �q�Z�C�V��
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LIGHT	g_Light[LIGHT_MAX];		//���C�g�𐧌䂷�邽�߂̔z��Ō���5(LIGHT_MAX)��

static FOG		g_Fog;					//���i�t�H�O�j�̐���	

static BOOL		g_FogEnable = FALSE;			//�� ON/OFF

int Lighting=0;
float LightingF = 0;
bool LightType = false;
//=============================================================================
// ����������
//=============================================================================
void InitLight(void)
{

	//���C�g������
	for (int i = 0; i < LIGHT_MAX; i++)
	{
		g_Light[i].Position  = XMFLOAT3( 0.0f, 0.0f, 0.0f );
		g_Light[i].Direction = XMFLOAT3( 0.0f, -1.0f, 0.0f );
		g_Light[i].Diffuse   = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
		g_Light[i].Ambient   = XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
		g_Light[i].Attenuation = 150.0f;	// ��������
		g_Light[i].Type = LIGHT_TYPE_NONE;	// ���C�g�̃^�C�v
		g_Light[i].Enable = false;			// ON / OFF
		SetLight(i, &g_Light[i]);
	}

	// ���s�����̐ݒ�i���E���Ƃ炷���j
	g_Light[0].Direction = XMFLOAT3( 0.0f, -1.0f, -1.0f );		// ���̌���
	g_Light[0].Diffuse   = XMFLOAT4(0.9f, 0.5f, 0.2f, 1.0f);	// ���̐F
	g_Light[0].Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.3f);
	g_Light[0].Type = LIGHT_TYPE_DIRECTIONAL;					// ���s����
	g_Light[0].Enable = true;									// ���̃��C�g��ON
	SetLight(0, &g_Light[0]);									// ����Őݒ肵�Ă���
	//      ��        ��
	//  ���C�g�ԍ�   �Z�b�g�������z��̃A�h���X

		// ���s�����̐ݒ�i���E���Ƃ炷���j
	g_Light[1].Direction = XMFLOAT3(0.0f, -1.0f, 1.0f);		// ���̌���
	g_Light[1].Diffuse = XMFLOAT4(0.8f, 0.8f, 1.0f, 1.0f);	// ���̐F
	g_Light[1].Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.3f);
	g_Light[1].Type = LIGHT_TYPE_DIRECTIONAL;					// ���s����
	g_Light[1].Enable = true;									// ���̃��C�g��ON
	SetLight(1, &g_Light[1]);									// ����Őݒ肵�Ă���


	//g_Light[1].Position = XMFLOAT3(-50.0f, 60.0f, 50.0f);		// ���̃|�W�V����
	//g_Light[1].Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);			// ���̌���
	//g_Light[1].Diffuse = XMFLOAT4(0.2f, 1.0f, 0.2f, 1.0f);		// ���̐F
	//g_Light[1].Ambient = XMFLOAT4(0.2f, 1.0f, 0.2f, 1.0f);
	//g_Light[1].Type = LIGHT_TYPE_POINT;							// �_����
	//g_Light[1].Enable = true;									// ���̃��C�g��ON
	//SetLight(1, &g_Light[1]);									// ����Őݒ肵�Ă���

	//// �_�����̐ݒ�
	//g_Light[2].Position = XMFLOAT3(50.0f, 60.0f, 50.0f);		// ���̃|�W�V����
	//g_Light[2].Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);			// ���̌���
	//g_Light[2].Diffuse = XMFLOAT4(1.0f, 0.2f, 0.2f, 1.0f);		// ���̐F
	//g_Light[2].Type = LIGHT_TYPE_POINT;							// �_����
	//g_Light[2].Enable = true;									// ���̃��C�g��ON
	//SetLight(2, &g_Light[2]);

	//// �_�����̐ݒ�
	//g_Light[3].Position = XMFLOAT3(0.0f, 60.0f, -40.0f);		// ���̃|�W�V����
	//g_Light[3].Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);			// ���̌���
	//g_Light[3].Diffuse = XMFLOAT4(0.2f, 0.2f, 1.0f, 1.0f);		// ���̐F
	//g_Light[3].Type = LIGHT_TYPE_POINT;							// �_����
	//g_Light[3].Enable = true;									// ���̃��C�g��ON
	//SetLight(3, &g_Light[3]);

	//// �_�����̐ݒ�
	//g_Light[4].Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);			// ���̌���
	//g_Light[4].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);		// ���̐F
	//g_Light[4].Type = LIGHT_TYPE_POINT;							// �_����
	//g_Light[4].Enable = true;									// ���̃��C�g��ON
	//SetLight(4, &g_Light[4]);

	//// �_�����̐ݒ�
	//g_Light[5].Position = XMFLOAT3(-100.0f, 60.0f, -50.0f);			// ���̌���
	//g_Light[5].Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);			// ���̌���
	//g_Light[5].Diffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);		// ���̐F
	//g_Light[5].Type = LIGHT_TYPE_POINT;							// �_����
	//g_Light[5].Enable = true;									// ���̃��C�g��ON
	//SetLight(5, &g_Light[5]);

	// �t�H�O�̏������i���̌��ʁj
	g_Fog.FogStart = 50.0f;									// ���_���炱�̋��������ƃt�H�O��������n�߂�
	g_Fog.FogEnd   = 400.0f;									// �����܂ŗ����ƃt�H�O�̐F�Ō����Ȃ��Ȃ�
	g_Fog.FogColor = XMFLOAT4{ 0.2f, 0.5f, 0.6f, 0.5f};		// �t�H�O�̐F
	SetFog(&g_Fog);
	SetFogEnable(g_FogEnable);				// ���̏ꏊ���`�F�b�N���� shadow

}


//=============================================================================
// �X�V����
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

				g_Light[i].Diffuse = XMFLOAT4(red, green, blue, 1.0f);		// ���̐F

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

			g_Light[i].Diffuse = XMFLOAT4(red, green, blue, 1.0f);		// ���̐F

			SetLight(i, &g_Light[i]);
		}
	}

}


//=============================================================================
// ���C�g�̐ݒ�
// Type�ɂ���ăZ�b�g���郁���o�[�ϐ����ς���Ă���
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
// �t�H�O�̐ݒ�
//=============================================================================
void SetFogData(FOG *fog)
{
	SetFog(fog);
}


BOOL	GetFogEnable(void)
{
	return(g_FogEnable);
}


