//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : �q�Z�C�V��
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(11)				// �e�N�X�`���̐�

#define CHECKMODE					(3)				// ���[�h�̐�
#define BACKGROUND					(4)				// BG�̐�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/bg001.png",
	"data/TEXTURE/bg002.png",
	"data/TEXTURE/bg003.png",
	"data/TEXTURE/bg004.png",
	"data/TEXTURE/newgame.png",
	"data/TEXTURE/newgame_check.png",
	"data/TEXTURE/loadgame.png",
	"data/TEXTURE/loadgame_check.png",
	"data/TEXTURE/quit.png",
	"data/TEXTURE/quit_check.png",
	"data/TEXTURE/title.png",

};

static BOOL					g_Load = FALSE;
static TITLE				g_Title[BACKGROUND];


int checkmode = 0;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitle(void)
{
	ID3D11Device* pDevice = GetDevice();

	//�e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �ϐ��̏�����
	for (int i = 0; i < BACKGROUND; i++)
	{
		g_Title[i].w = TEXTURE_WIDTH * 1.6;
		g_Title[i].h = TEXTURE_HEIGHT;
		g_Title[i].pos = XMFLOAT3(TEXTURE_WIDTH /2, TEXTURE_HEIGHT/2, 0.0f);
		g_Title[i].scrl = 0;
	}
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitle(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTitle(void)
{
	g_Title[0].scrl += 0.002f;
	g_Title[1].scrl += 0.0015f;
	g_Title[2].scrl += 0.001f;
	g_Title[3].scrl += 0.001f;

	if (GetKeyboardTrigger(DIK_DOWN))
	{
		PlaySound(SOUND_LABEL_SE_button);
		checkmode++;
	}
	else if (GetKeyboardTrigger(DIK_UP))
	{
		PlaySound(SOUND_LABEL_SE_button);
		if (checkmode > 0)
		{
			checkmode--;
		}
		else
		{
			checkmode = CHECKMODE - 1;
		}
	}

	checkmode %= CHECKMODE;

	if (GetKeyboardTrigger(DIK_RETURN))
	{// Enter��������A�X�e�[�W��؂�ւ���
		PlaySound(SOUND_LABEL_SE_button);
		switch (checkmode)
		{
		case 0:
			SetFade(FADE_OUT, MODE_TUTORIAL);
			break;
		case 1:
			//SetLoadGame(TRUE);
			SetFade(FADE_OUT, MODE_TUTORIAL);
			break;
		case 2:
			exit(0);
			break;
		}
	}




#ifdef _DEBUG	// �f�o�b�O����\������
	//PrintDebugProc("Player:�� �� �� ���@Space\n");

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitle(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// �^�C�g���̔w�i��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer,
			g_Title[0].pos.x, g_Title[0].pos.y, g_Title[0].w, g_Title[0].h,
			g_Title[0].scrl, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}
	// �^�C�g���̔w�i��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer,
			g_Title[1].pos.x, g_Title[1].pos.y, g_Title[1].w, g_Title[1].h,
			g_Title[1].scrl, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}
	// �^�C�g���̔w�i��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer,
			g_Title[2].pos.x, g_Title[2].pos.y, g_Title[2].w, g_Title[2].h,
			g_Title[2].scrl, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}
	// �^�C�g���̔w�i��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer,
			g_Title[3].pos.x, g_Title[3].pos.y, g_Title[3].w, g_Title[3].h,
			g_Title[3].scrl, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	switch (checkmode)
	{
	case 0:
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[4]);
		SetSpriteLeftTop(g_VertexBuffer, 650.0f, 200.0f, 300, 240, 0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[7]);
		SetSpriteLeftTop(g_VertexBuffer, 650.0f, 270.0f, 300, 240, 0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[9]);
		SetSpriteLeftTop(g_VertexBuffer, 650.0f, 340.0f, 300, 240, 0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
		break;
	case 1:
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[5]);
		SetSpriteLeftTop(g_VertexBuffer, 650.0f, 200.0f, 300, 240, 0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[6]);
		SetSpriteLeftTop(g_VertexBuffer, 650.0f, 270.0f, 300, 240, 0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[9]);
		SetSpriteLeftTop(g_VertexBuffer, 650.0f, 340.0f, 300, 240, 0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
		break;
	case 2:
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[5]);
		SetSpriteLeftTop(g_VertexBuffer, 650.0f, 200.0f, 300, 240, 0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[7]);
		SetSpriteLeftTop(g_VertexBuffer, 650.0f, 270.0f, 300, 240, 0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[8]);
		SetSpriteLeftTop(g_VertexBuffer, 650.0f, 340.0f, 300, 240, 0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
		break;
	}

	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[10]);
		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0, 960, 440, 0.0f, 0.0f, 1.0f, 1.0f);
		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}
}



