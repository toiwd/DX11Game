#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "mouse.h"
#include "scene.h"
#include "model.h"
#include "titleLogo3D.h"

Mouse_State tMouse;

void TitleLogo3D::Init()
{
	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/earthenvmap.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);

	m_Model = new Model();
	m_Model->Load("asset\\model\\title.obj");

	m_Position = D3DXVECTOR3(0.0f, 3.0f, -14.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.5f, 1.5f, 1.5f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "envMappingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "envMappingPS.cso");
}

void TitleLogo3D::Uninit()
{
	m_Texture->Release();
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void TitleLogo3D::Update()
{
	// ��΍��W���[�h�ɕύX
	Mouse_SetMode(MOUSE_POSITION_MODE_ABSOLUTE);

	// �}�E�X�̌��݂̏�Ԃ�ێ�
	Mouse_GetState(&tMouse);

	if (Input::GetKeyTrigger(VK_LBUTTON))
	{
		m_MouseDownPosition.y = tMouse.y;
	}

	if (Input::GetKeyPress(VK_LBUTTON))
	{
		m_Position.x = tMouse.x / 100.0f -5.0f;
		m_Position.y = - tMouse.y / 100.0f + 5.0f;
		m_Velocity.x = 0.0f;
		m_Velocity.y = 0.0f;
	}
	else
	{
		float dt = 0.06f;
		

		D3DXVECTOR2 f;

		f.x = 0.0f;
		f.y = 0.0f;

		// ����
		f.x += (0.0f - m_Position.x) * 1.0f;
		f.y += (-7.0f - m_Position.y) * 1.0f;

		// �d��
		f.y += m_Gravity * m_Mass;

		// ��R
		f.x -= m_Velocity.x * 0.1f;
		f.y -= m_Velocity.y * 0.1f;

		// ���͂ɂ�����
		m_Velocity.x += (f.x / m_Mass) * dt;
		m_Velocity.y += (f.y / m_Mass) * dt;

		// �ړ�
		m_Position.x += m_Velocity.x * dt;
		m_Position.y += m_Velocity.y * dt;
	}
	
	m_Rotation.y += 0.01f;
}

void TitleLogo3D::Draw()
{
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_Texture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);

	m_Model->Draw();
}