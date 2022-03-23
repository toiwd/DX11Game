#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "field.h"
#include "scene.h"
#include "titleCamera.h"

void Field::Init()
{
	VERTEX_3D vertex[4];

	//����
	vertex[0].Position = D3DXVECTOR3(-5.0f, 5.0f+5.0f, -13.0f);			//�`����W
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//���C�e�B���O
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	//�F
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//�e�N�X�`�����W

	//�E��
	vertex[1].Position = D3DXVECTOR3(5.0f, 5.0f + 5.0f, -13.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(10.0f, 0.0f);

	//����
	vertex[2].Position = D3DXVECTOR3(-5.0f, -5.0f + 5.0f, -13.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 10.0f);

	//�E��
	vertex[3].Position = D3DXVECTOR3(5.0f, -5.0f + 5.0f, -13.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(10.0f, 10.0f);


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;	//�T�C�Y�̐ݒ�
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//VERTEX_BUFFER�Ƃ�������
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};//�����l�̐ݒ�
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/field001.jpg",//�t�@�C���w��
		NULL,
		NULL,
		&m_Texture,					//�o���オ�����e�N�X�`��
		NULL);
	assert(m_Texture);	//�G���[�`�F�b�N(NULL���Ɨ�O�X���[)

	//�R���p�C���ς݂̃V�F�[�_�[�v���O������ǂݍ���ŁAVertexShader�ɓ��ꍞ��
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");

	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

void Field::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Field::Update()
{
	
}

void Field::Draw()
{
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);	//FVF�̑���

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);	//�}�e���A���ɂ���ăV�F�[�_���ς��
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// ���[���h�}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer,	&stride, &offset);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);//���_��
}