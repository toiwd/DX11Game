#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "fade.h"
#include "title.h"
#include "game.h"
#include "result.h"

#define	FADE_RATE	(0.02f)		// �t�F�[�h�W���i���邭�܂��͈Â��Ȃ鑬�x�j

ID3D11VertexShader*			Fade::m_VertexShader = NULL;
ID3D11PixelShader*			Fade::m_PixelShader = NULL;
ID3D11InputLayout*			Fade::m_VertexLayout = NULL;

ID3D11Buffer*				Fade::m_VertexBuffer = NULL;
ID3D11ShaderResourceView*	Fade::m_Texture = NULL;

int		Fade::m_Fade = 0;
int		Fade::m_NextScene = 0;
float	Fade::m_FadeLight = 0.0f;

void Fade::Init()
{
	VERTEX_3D vertex[4];

	//����
	vertex[0].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�`����W
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.0f);	//�F
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//�e�N�X�`�����W

	//�E��
	vertex[1].Position = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	//����
	vertex[2].Position = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	//�E��
	vertex[3].Position = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.0f);
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
		"asset/texture/fadeblack.png",//�t�@�C���w��
		NULL,
		NULL,
		&m_Texture,					//�o���オ�����e�N�X�`��
		NULL);
	assert(m_Texture);	//�G���[�`�F�b�N(NULL���Ɨ�O�X���[)

	//�R���p�C���ς݂̃V�F�[�_�[�v���O������ǂݍ���ŁAVertexShader�ɓ��ꍞ��
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,"unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");
}


void Fade::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Fade::Update()
{
	if (m_Fade == 1) // �t�F�[�h�A�E�g
	{
		// �����ňÂ�����
		m_FadeLight += FADE_RATE;

		// ���_�f�[�^��������
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		//����
		vertex[0].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�`����W
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, m_FadeLight);	//�F
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//�e�N�X�`�����W

		//�E��
		vertex[1].Position = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, m_FadeLight);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

		//����
		vertex[2].Position = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, m_FadeLight);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

		//�E��
		vertex[3].Position = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, m_FadeLight);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	}
	else if (m_Fade == 2) // �t�F�[�h�C��
	{
		// �����Ŗ��邭����
		m_FadeLight -= FADE_RATE;

		// ���_�f�[�^��������
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		//����
		vertex[0].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�`����W
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, m_FadeLight);	//�F
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//�e�N�X�`�����W

		//�E��
		vertex[1].Position = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, m_FadeLight);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

		//����
		vertex[2].Position = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, m_FadeLight);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

		//�E��
		vertex[3].Position = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, m_FadeLight);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

		// ���S�ɖ��邭�Ȃ����珈�����~�߂�
		if (m_FadeLight < 0.0f)
		{
			m_Fade = 0;
		}
	}

	// ���S�ɈÂ��Ȃ����玟�̃V�[���ֈڍs
	if (m_FadeLight > 1.0f)
	{
		switch (m_NextScene)
		{
		case 1:
			Manager::SetScene<Title>();
			m_Fade = 2;
			break;
		case 2:
			Manager::SetScene<Game>();
			m_Fade = 2;
			break;
		case 3:
			Manager::SetScene<Result>();
			m_Fade = 2;
			break;
		default:
			break;
		}
	}
}

void Fade::Draw()
{
	// �t�F�[�h���I����Ă�����`�悵�Ȃ�
	if (m_Fade == 0) return

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
