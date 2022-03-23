#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "model.h"
#include "bomb.h"
#include "player.h"
#include "enemy.h"
#include "block.h"
#include "ink.h"
#include "bombSplash.h"
#include "explosion.h"

Model* Bomb::m_Model;//�X�^�e�B�b�N�����o�ϐ���cpp�ōēx�錾���K�v

#define BULLET_JUMP (0.10f)	// �e�̔�ԍ���

void Bomb::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\bomb.obj");
}

void Bomb::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void Bomb::Init()
{
	m_Position = D3DXVECTOR3(-2.0f, 1.5f, -1.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.4f, 0.4f, 0.4f);

	m_Jump_v0 = BULLET_JUMP;
	m_Gravity = -0.010f;
	m_range = 0.6f;
	m_Speed = { 0.0f,0.0f,0.0f };
	m_BombTime = 0.0f;

	// �����x�̐ݒ�
	m_Speed.y = m_Jump_v0;
	m_Speed.x = m_range;

	// �v���C���[�̌����𔽉f������
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(1);
	D3DXVECTOR3 playerRotation = player->GetRotation();
	m_Rotation = playerRotation;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");
}

void Bomb::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Bomb::Update()
{
	Scene* scene = Manager::GetScene();
	std::vector<Enemy*> enemyList = scene->GetGameObjects<Enemy>(1);
	std::vector<Block*> blockList = scene->GetGameObjects<Block>(1);
	std::vector<Ink*> inkList = scene->GetGameObjects<Ink>(1);
	D3DXVECTOR3 forward = GetForward();

	// ���ʂɌ������Ĕ��ł���
	if (m_Speed.x > 0.0f)
	{
		m_Position += forward * m_Speed.x;
		m_Speed.x += m_Gravity;
	}

	// �n�ʂɗ������
	if (m_Position.y <= 0.0f)
	{
		m_Position.y = 0.0f;
		m_Gravity = -0.020f;
	}
	else
	{
		// �d��
		// Y�����̑��x�ɉ����x��������
		m_Speed.y += m_Gravity;
		
		// Y���W�̍X�V
		m_Position.y += m_Speed.y;
	}

	// ��莞�Ԍo������{��������
	if (m_BombTime >= 100.0f)
	{
		// �򖗂��΂�
		for (int i = 0; i < 50; i++)
		{
			scene->AddGameObject<BombSplash>(1)->SetPosition(D3DXVECTOR3(m_Position.x, 1.0f, m_Position.z));
		}
		SetDestroy();	// �����\��

		return;
	}

	m_BombTime += 1.0f;
}

void Bomb::Draw()
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

	m_Model->Draw();
}