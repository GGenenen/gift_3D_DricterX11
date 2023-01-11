//=============================================================================
//
// �J�������� [camera.cpp]
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "player.h"
#include "game.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VIEW_ANGLE		(XMConvertToRadians(45.0f))						// �r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��	
#define	VIEW_NEAR_Z		(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z		(10000.0f)										// �r���[���ʂ�FarZ�l

#define	VALUE_MOVE_CAMERA	(2.0f)										// �J�����̈ړ���
#define	VALUE_ROTATE_CAMERA	(XM_PI * 0.01f)								// �J�����̉�]��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static CAMERA			g_Camera;		// �J�����f�[�^

static int				g_ViewPortType = TYPE_FULL_SCREEN;

float g_Camera_at_rot = 0.0f;

XMFLOAT3 OnePerson = {8.0f,10.0f,8.0f };
XMFLOAT3 ThirdPerson = { 70.0f,20.0f,70.0f };

//�l�̃t���O true:�O�l�� false:��l��
bool ViewType = false;

//=============================================================================
// ����������
//=============================================================================
void InitCamera(void)
{
	g_Camera.pos = { 0.0f, 0.0f, 0.0f };
	g_Camera.at = { 0.0f, 0.0f, 0.0f };
	g_Camera.up = { 0.0f, 1.0f, 0.0f };
	g_Camera.rot = { 0.0f, 0.0f, 0.0f };

	// ���_�ƒ����_�̋������v�Z
	g_Camera.len = 200.0f;

	// �r���[�|�[�g�^�C�v�̏�����
	g_ViewPortType = TYPE_FULL_SCREEN;
}


//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{

}


//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{
	//�}�E�X�̈ړ�����
	{
		long diff_X = GetMousePosX() - SCREEN_CENTER_X; //diff_X > 0  �E��  diff_X < 0  ����
		long diff_Y = GetMousePosY() - SCREEN_CENTER_Y; //diff_Y > 0  ����  diff_Y < 0  �㑤

		g_Camera.rot.y += XM_PI * diff_X * 0.005f;

		if (g_Camera_at_rot <= XM_PI / 2 && g_Camera_at_rot >= -XM_PI / 4)
		{
			g_Camera_at_rot += XM_PI * diff_Y * 0.002f;
		}
		else if (g_Camera_at_rot > XM_PI / 2)
		{
			g_Camera_at_rot = XM_PI / 2;
		}
		else
		{
			g_Camera_at_rot = -XM_PI / 4;
		}
	}

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F3))
	{
		//�l�̃t���O true:�O�l�� false:��l��
		ViewType = ViewType ? false : true;
	}
#endif

	if (ViewType)
	{
		//�O�l��
		g_Camera.pos.x = GetPlayer()->pos.x - sinf(g_Camera.rot.y) * ThirdPerson.x;
		g_Camera.pos.y = GetPlayer()->pos.y + ThirdPerson.y;
		g_Camera.pos.z = GetPlayer()->pos.z - cosf(g_Camera.rot.y) * ThirdPerson.z;
	}
	else
	{
		//��l�́@DirectX�̎����J�������O������,+ XM_PI
		g_Camera.pos.x = GetPlayer()->pos.x - sinf(g_Camera.rot.y + XM_PI) * OnePerson.x;
		g_Camera.pos.y = GetPlayer()->pos.y + OnePerson.y;
		g_Camera.pos.z = GetPlayer()->pos.z - cosf(g_Camera.rot.y + XM_PI) * OnePerson.z;
	}

	//DirectX�̎��������_���O������, +XM_PI
	g_Camera.at.x = GetPlayer()->pos.x - sinf(g_Camera.rot.y + XM_PI) * g_Camera.len;
	g_Camera.at.y = GetPlayer()->pos.y - sinf(g_Camera_at_rot) * g_Camera.len;
	g_Camera.at.z = GetPlayer()->pos.z - cosf(g_Camera.rot.y + XM_PI) * g_Camera.len;

	SetCamera();

	//�J�[�\���𒆐S�ɐݒ�
	//SetCursorPos(SCREEN_CENTER_X, SCREEN_CENTER_Y);

#ifdef _DEBUG

#endif

}


//=============================================================================
// �J�����̍X�V
//=============================================================================
void SetCamera(void)
{
	// �r���[�}�g���b�N�X�ݒ�
	XMMATRIX mtxView;
	mtxView = XMMatrixLookAtLH(XMLoadFloat3(&g_Camera.pos), XMLoadFloat3(&g_Camera.at), XMLoadFloat3(&g_Camera.up));
	SetViewMatrix(&mtxView);
	XMStoreFloat4x4(&g_Camera.mtxView, mtxView);

	XMMATRIX mtxInvView;
	mtxInvView = XMMatrixInverse(nullptr, mtxView);
	XMStoreFloat4x4(&g_Camera.mtxInvView, mtxInvView);


	// �v���W�F�N�V�����}�g���b�N�X�ݒ�
	XMMATRIX mtxProjection;
	mtxProjection = XMMatrixPerspectiveFovLH(VIEW_ANGLE, VIEW_ASPECT, VIEW_NEAR_Z, VIEW_FAR_Z);

	SetProjectionMatrix(&mtxProjection);
	XMStoreFloat4x4(&g_Camera.mtxProjection, mtxProjection);

	SetShaderCamera(g_Camera.pos);
}


//=============================================================================
// �J�����̎擾
//=============================================================================
CAMERA* GetCamera(void)
{
	return &g_Camera;
}

//=============================================================================
// �r���[�|�[�g�̐ݒ�
//=============================================================================
void SetViewPort(int type)
{
	ID3D11DeviceContext* g_ImmediateContext = GetDeviceContext();
	D3D11_VIEWPORT vp;

	g_ViewPortType = type;

	// �r���[�|�[�g�ݒ�
	switch (g_ViewPortType)
	{
	case TYPE_FULL_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH;
		vp.Height = (FLOAT)SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		break;

	case TYPE_LEFT_HALF_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH / 2;
		vp.Height = (FLOAT)SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		break;

	case TYPE_RIGHT_HALF_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH / 2;
		vp.Height = (FLOAT)SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = (FLOAT)SCREEN_WIDTH / 2;
		vp.TopLeftY = 0;
		break;

	case TYPE_UP_HALF_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH;
		vp.Height = (FLOAT)SCREEN_HEIGHT / 2;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		break;

	case TYPE_DOWN_HALF_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH;
		vp.Height = (FLOAT)SCREEN_HEIGHT / 2;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = (FLOAT)SCREEN_HEIGHT / 2;
		break;


	}
	g_ImmediateContext->RSSetViewports(1, &vp);

}


int GetViewPortType(void)
{
	return g_ViewPortType;
}



// �J�����̎��_�ƒ����_���Z�b�g
void SetCameraAT(XMFLOAT3 pos)
{
	// �J�����̒����_�������̍��W�ɂ��Ă݂�
	g_Camera.at = pos;

	//// �J�����̎��_���J������Y����]�ɑΉ������Ă���
	//g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
	//g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;

}

