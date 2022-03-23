#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "mouse.h"
#include "option.h"
#include "optionFrame.h"


ID3D11VertexShader*			OptionFrame::m_VertexShader = NULL;
ID3D11PixelShader*			OptionFrame::m_PixelShader = NULL;
ID3D11InputLayout*			OptionFrame::m_VertexLayout = NULL;

ID3D11Buffer*				OptionFrame::m_VertexBuffer = NULL;
ID3D11ShaderResourceView*	OptionFrame::m_Texture = NULL;

int							OptionFrame::m_SelectFrame = 0;

Mouse_State ofMouse;

void OptionFrame::Init()
{
	VERTEX_3D vertex[4];

	//����
	vertex[0].Position = D3DXVECTOR3(30.0f, 345.0f, 0.0f);		//�`����W
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	//�F
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//�e�N�X�`�����W

	//�E��
	vertex[1].Position = D3DXVECTOR3(310.0f, 345.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	//����
	vertex[2].Position = D3DXVECTOR3(30.0f, 525.0f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	//�E��
	vertex[3].Position = D3DXVECTOR3(310.0f, 525.0f, 0.0f);
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
		"asset/texture/optionframe.png",//�t�@�C���w��
		NULL,
		NULL,
		&m_Texture,					//�o���オ�����e�N�X�`��
		NULL);
	assert(m_Texture);	//�G���[�`�F�b�N(NULL���Ɨ�O�X���[)

	//�R���p�C���ς݂̃V�F�[�_�[�v���O������ǂݍ���ŁAVertexShader�ɓ��ꍞ��
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,"unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");
}


void OptionFrame::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void OptionFrame::Update()
{
	// �}�E�X�̌��݂̏�Ԃ�ێ�
	Mouse_GetState(&ofMouse);

	if (30.0f <= (float)ofMouse.x && (float)ofMouse.x <= 310.0f &&
		345.0f <= (float)ofMouse.y && (float)ofMouse.y <= 525.0f)
	{
		// ���_�f�[�^��������
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		//����
		vertex[0].Position = D3DXVECTOR3(30.0f, 345.0f, 0.0f);		//�`����W
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	//�F
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//�e�N�X�`�����W

		//�E��
		vertex[1].Position = D3DXVECTOR3(310.0f, 345.0f, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

		//����
		vertex[2].Position = D3DXVECTOR3(30.0f, 525.0f, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

		//�E��
		vertex[3].Position = D3DXVECTOR3(310.0f, 525.0f, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
		
		// �^�C�g���֖߂�
		m_SelectFrame = 1;
	}
	else if (30.0f + 310.0f <= (float)ofMouse.x && (float)ofMouse.x <= 310.0f + 310.0f && 345.0f <= (float)ofMouse.y && (float)ofMouse.y <= 525.0f)
	{
		// ���_�f�[�^��������
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		//����
		vertex[0].Position = D3DXVECTOR3(30.0f+310.0f, 345.0f, 0.0f);	//�`����W
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);		//�F
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);					//�e�N�X�`�����W

		//�E��
		vertex[1].Position = D3DXVECTOR3(310.0f + 310.0f, 345.0f, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

		//����
		vertex[2].Position = D3DXVECTOR3(30.0f + 310.0f, 525.0f, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

		//�E��
		vertex[3].Position = D3DXVECTOR3(310.0f + 310.0f, 525.0f, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

		// �Q�[�������Z�b�g����
		m_SelectFrame = 2;
	}
	else if (30.0f + 620.0f <= (float)ofMouse.x && (float)ofMouse.x <= 310.0f + 620.0f && 345.0f <= (float)ofMouse.y && (float)ofMouse.y <= 525.0f)
	{
		// ���_�f�[�^��������
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		//����
		vertex[0].Position = D3DXVECTOR3(30.0f + 620.0f, 345.0f, 0.0f);			//�`����W
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	//�F
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//�e�N�X�`�����W

		//�E��
		vertex[1].Position = D3DXVECTOR3(310.0f + 620.0f, 345.0f, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

		//����
		vertex[2].Position = D3DXVECTOR3(30.0f + 620.0f, 525.0f, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

		//�E��
		vertex[3].Position = D3DXVECTOR3(310.0f + 620.0f, 525.0f, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

		// �Q�[���ɖ߂�
		m_SelectFrame = 3;
	}
	else
	{
		m_SelectFrame = 0;
	}

	if (Input::GetKeyTrigger(VK_LBUTTON))
	{
		switch (m_SelectFrame)
		{
		case 1:	// �^�C�g���ɖ߂�
			Fade::SetFade(1, 1);
			Option::SetOptionMode(false);
			break;
		case 2: // �Q�[�������Z�b�g����
			Fade::SetFade(1, 2);
			Option::SetOptionMode(false);
			break;
		case 3: // �Q�[���ɖ߂�
			Option::SetOptionMode(false);
			break;
		default:
			break;
		}
	}
	
}

void OptionFrame::Draw()
{
	// �I�v�V������ԂłȂ���Ε`�悵�Ȃ�
	if (m_SelectFrame == 0) return;

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

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);//���_��
}
