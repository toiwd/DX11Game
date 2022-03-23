#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "mouse.h"
#include "option.h"
#include "optionbar.h"
#include "player.h"
#include "Audio.h"

float	OptionBar::m_BarPointBGM = 600.0f;
float	OptionBar::m_BarPointSE = 600.0f;
bool	OptionBar::m_PointChange = false;

Mouse_State obMouse;

void OptionBar::Init(int Num)
{
	VERTEX_3D vertex[4];

	m_Position.x = 237.5f + 600.0f;
	// 2�̂Ƃ������^�񒆂ɃZ�b�g����
	if (Num == 2)
	{
		m_Position.x = 237.5f + 300.0f;
	}
	m_Position.y = 75.0f + Num * 100.0f;

	//����
	vertex[0].Position = D3DXVECTOR3(0.0f + m_Position.x, 0.0f + m_Position.y, 0.0f);			//�`����W
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	//�F
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//�e�N�X�`�����W

	//�E��
	vertex[1].Position = D3DXVECTOR3(25.0f + m_Position.x, 0.0f + m_Position.y, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	//����
	vertex[2].Position = D3DXVECTOR3(0.0f + m_Position.x, 50.0f + m_Position.y, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	//�E��
	vertex[3].Position = D3DXVECTOR3(25.0f + m_Position.x, 50.0f + m_Position.y, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;	//�T�C�Y�̐ݒ�
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//VERTEX_BUFFER�Ƃ�������
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};//�����l�̐ݒ�
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/optionbar.png",//�t�@�C���w��
		NULL,
		NULL,
		&m_Texture,					//�o���オ�����e�N�X�`��
		NULL);
	assert(m_Texture);	//�G���[�`�F�b�N(NULL���Ɨ�O�X���[)

	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/optionbar2.png",//�t�@�C���w��
		NULL,
		NULL,
		&m_Texture2,					//�o���オ�����e�N�X�`��
		NULL);
	assert(m_Texture2);	//�G���[�`�F�b�N(NULL���Ɨ�O�X���[)


	//DirectX�̊֐��ł͂Ȃ�
	//�R���p�C���ς݂̃V�F�[�_�[�v���O������ǂݍ���ŁAVertexShader�ɓ��ꍞ��
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"unlitTextureVS.cso");
	//DirectX�̊֐��ł͂Ȃ�
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");

	//unlit�Ƃ́����̌v�Z�����Ȃ��Ƃ����Ӗ�
}


void OptionBar::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void OptionBar::Update(int Num)
{
	// �}�E�X�̌��݂̏�Ԃ�ێ�
	Mouse_GetState(&obMouse);

	// �e�N�X�`����Ƀ}�E�X�����邩
	if (m_Position.x - 10.0f <= obMouse.x && m_Position.x + 30.0f >= obMouse.x && m_Position.y-5.0f <= obMouse.y && m_Position.y + 55.0f >= obMouse.y)
	{
		// ���ɑ��̃o�[���͂܂�Ă��Ȃ��ꍇ
		if (!Input::GetKeyPress(VK_LBUTTON))
		{
			m_OnMouse = true;
		}
		
		if (Input::GetKeyTrigger(VK_LBUTTON))
		{
			m_HoldMouse = true;
		}
	}
	else
	{
		m_OnMouse = false;
	}

	// �͂񂾃o�[�𗣂�����
	if (!Input::GetKeyPress(VK_LBUTTON))
	{
		m_HoldMouse = false;
		
	}

	// �͂�ł����瓮������悤�ɂ���
	if (m_HoldMouse == true)
	{
		m_Position.x = obMouse.x;
		if (m_Position.x >= 600.0f + 237.5f)
		{
			m_Position.x = 600.0f + 237.5f;
		}
		else if (m_Position.x <= 237.5f)
		{
			m_Position.x = 237.5f;
			// 0�ɂȂ�Ȃ��悤�ɂ���
			if (Num == 2)
			{
				m_Position.x = 238.5f;
			}
		}

		// ���_�f�[�^��������
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		//����
		vertex[0].Position = D3DXVECTOR3(0.0f + m_Position.x, 0.0f + m_Position.y, 0.0f);			//�`����W
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	//�F
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//�e�N�X�`�����W

		//�E��
		vertex[1].Position = D3DXVECTOR3(25.0f + m_Position.x, 0.0f + m_Position.y, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

		//����
		vertex[2].Position = D3DXVECTOR3(0.0f + m_Position.x, 50.0f + m_Position.y, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

		//�E��
		vertex[3].Position = D3DXVECTOR3(25.0f + m_Position.x, 50.0f + m_Position.y, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);



		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

		m_PointChange = true;
	}

	// �o�[�̈ʒu�ɉ��������l
	m_BarPoint = m_Position.x - 237.5f;

	// Num�̐��l�ɉ������ύX
	if (Num == 0)
	{
		m_BarPointBGM = m_Position.x - 237.5f;
	}
	else if (Num == 1)
	{
		m_BarPointSE = m_Position.x - 237.5f;
	}
	else
	{
		Player::SetPlayerSens(m_BarPoint);
	}
}

void OptionBar::Draw()
{
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);	//FVF�̑���

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);	//�}�e���A���ɂ���ăV�F�[�_���ς��
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();	//2D�p�̃}�g���N�X

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	if (m_OnMouse == true || m_HoldMouse == true)
	{
		// �e�N�X�`���ݒ�
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	}
	else
	{
		// �e�N�X�`���ݒ�
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture2);
	}
	
	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);//���_��
}
