#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "HpBar.h"
#include "player.h"
#include "camera.h"

ID3D11Buffer*				HpBar::m_VertexBuffer = NULL;
ID3D11ShaderResourceView*	HpBar::m_Texture = NULL;

void HpBar::Load()
{
	VERTEX_3D vertex[4];

	//����
	vertex[0].Position = D3DXVECTOR3(-1.0f, 2.0f, 0.0f);		//�`����W
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//���C�e�B���O
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.0f);	//�F
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//�e�N�X�`�����W

	//�E��
	vertex[1].Position = D3DXVECTOR3(1.0f, 2.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	//����
	vertex[2].Position = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	//�E��
	vertex[3].Position = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;//�e�N�X�`���A�j���[�V�����ł͓��I��DYNAMIC�ɂ���
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;	//�T�C�Y�̐ݒ�
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//VERTEX_BUFFER�Ƃ�������
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//�e�N�X�`���A�j���[�V�����ł͏��������\��WRITE�ɂ���

	D3D11_SUBRESOURCE_DATA sd{};//�����l�̐ݒ�
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/enemyHpBar.png",//�t�@�C���w��
		NULL,
		NULL,
		&m_Texture,					//�o���オ�����e�N�X�`��
		NULL);
	assert(m_Texture);	//�G���[�`�F�b�N(NULL���Ɨ�O�X���[)
}

void HpBar::Unload()
{
	m_VertexBuffer->Release();
	m_Texture->Release();
}

void HpBar::Init()
{
	//�R���p�C���ς݂̃V�F�[�_�[�v���O������ǂݍ���ŁAVertexShader�ɓ��ꍞ��
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");
}

void HpBar::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void HpBar::Update()
{
	// �n�ʂɖ��܂�Ȃ��悤�ɂ���
	if (m_Position.y <= 0.0f)
	{
		m_Position.y = 0.0f;
	}
}

void HpBar::Draw()
{
	// �v���C���[�̏�Ԃ��擾����
	Player* player = Manager::GetScene()->GetGameObject<Player>(1);
	bool playerstate = player->GetPlayerState();
	float playerlife = player->GetLife();

	// ���_�f�[�^��������
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	//����
	vertex[0].Position = D3DXVECTOR3(-200.0f + 250.0f, -200.0f + 500.0f, 0.0f);			//�`����W
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);	//�F
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//�e�N�X�`�����W

	//�E��
	vertex[1].Position = D3DXVECTOR3(-200.0f + playerlife * 4.0f + 250.0f, -200.0f + 500.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f*playerlife/100.0f, 0.0f);

	//����
	vertex[2].Position = D3DXVECTOR3(-200.0f + 250.0f, 200.0f + 500.0f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	//�E��
	vertex[3].Position = D3DXVECTOR3(-200.0f + playerlife * 4.0f + 250.0f, 200.0f + 500.0f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f*playerlife / 100.0f, 1.0f);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

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