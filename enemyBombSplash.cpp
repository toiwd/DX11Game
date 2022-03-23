#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "model.h"
#include "enemyBombSplash.h"
#include "bomb.h"
#include "player.h"
#include "block.h"
#include "ink.h"
#include "enemyInk.h"
#include "explosion.h"
#include "meshField.h"

Model* EnemyBombSplash::m_Model;//�X�^�e�B�b�N�����o�ϐ���cpp�ōēx�錾���K�v

#define BULLET_JUMP (0.10f)	// �e�̔�ԍ���

void EnemyBombSplash::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\bullet.obj");
}

void EnemyBombSplash::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void EnemyBombSplash::Init()
{
	m_Position = D3DXVECTOR3(-2.0f, 1.5f, -1.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.1f, 0.1f, 0.1f);

	m_Jump_v0 = BULLET_JUMP;
	m_Gravity = -0.015f;
	m_range = (float)rand() / RAND_MAX * 0.55f;
	m_Speed = { 0.0f,0.0f,0.0f };

	// �����x�̐ݒ�
	m_Speed.y = m_Jump_v0;
	m_Speed.x = m_range;

	m_Rotation.y = (float)rand() / RAND_MAX * 7.0f;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");
}

void EnemyBombSplash::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void EnemyBombSplash::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(1);
	std::vector<Block*> blockList = scene->GetGameObjects<Block>(1);
	std::vector<EnemyInk*> inkList = scene->GetGameObjects<EnemyInk>(1);
	MeshField* meshField = scene->GetGameObject<MeshField>(1);
	D3DXVECTOR3 forward = GetForward();

	// ���ʂɌ������Ĕ��ł���
	m_Position += forward * m_Speed.x;

	// �d��
	// Y�����̑��x�ɉ����x��������
	m_Speed.y += m_Gravity;
	m_Speed.x += m_Gravity;
	// Y���W�̍X�V
	m_Position.y += m_Speed.y;

	// �n�ʂɗ�������e������
	if (m_Position.y < meshField->GetHeight(m_Position))
	{
		// �F��h��
		scene->AddGameObject<EnemyInk>(1)->SetPosition(D3DXVECTOR3(m_Position.x, m_Position.y + 0.2f, m_Position.z));
		//Ink::Paint(m_Position);

		std::vector<Ink*> inkList = scene->GetGameObjects<Ink>(1);

		// �߂��Ƀv���C���[�̃C���N������Ώ���
		for (Ink* ink : inkList)
		{
			D3DXVECTOR3 inkPosition = ink->GetPosition();
			D3DXVECTOR3 direction = m_Position - inkPosition;

			D3DXVECTOR3 obbx, obby, obbz;
			float obbLenx, obbLeny, obbLenz;

			obbx = ink->GetObbX(1.5f);
			obbLenx = D3DXVec3Length(&obbx);
			obbx /= obbLenx;

			obby = ink->GetObbY(1.0f);
			obbLeny = D3DXVec3Length(&obby);
			obby /= obbLeny;

			obbz = ink->GetObbZ(1.5f);
			obbLenz = D3DXVec3Length(&obbz);
			obbz /= obbLenz;

			float lenX, lenY, lenZ;
			lenX = D3DXVec3Dot(&obbx, &direction);
			lenY = D3DXVec3Dot(&obby, &direction);
			lenZ = D3DXVec3Dot(&obbz, &direction);

			if (fabs(lenX) < obbLenx && fabs(lenY) < obbLeny && fabs(lenZ) < obbLenz)
			{
				ink->SetDestroy();
				SetDestroy();	// �����\��
				return;
			}
		}

		SetDestroy();	// �����\��

		return;
	}

	if (player->GetDead() == false)
	{
		D3DXVECTOR3 playerPosition = player->GetPosition();
		D3DXVECTOR3 direction = m_Position - playerPosition;

		D3DXVECTOR3 obbx, obby, obbz;
		float obbLenx, obbLeny, obbLenz;

		obbx = player->GetObbX(1.2f);
		obbLenx = D3DXVec3Length(&obbx);
		obbx /= obbLenx;

		obby = player->GetObbY(2.4f);
		obbLeny = D3DXVec3Length(&obby);
		obby /= obbLeny;

		obbz = player->GetObbZ(1.2f);
		obbLenz = D3DXVec3Length(&obbz);
		obbz /= obbLenz;

		float lenX, lenY, lenZ;
		lenX = D3DXVec3Dot(&obbx, &direction);
		lenY = D3DXVec3Dot(&obby, &direction);
		lenZ = D3DXVec3Dot(&obbz, &direction);

		if (fabs(lenX) < obbLenx && fabs(lenY) < obbLeny && fabs(lenZ) < obbLenz)
		{
			scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);

			player->m_Life -= 50.0f;

			SetDestroy();
			return;
		}
	}

	// �u���b�N�ɓ�����ƒe����������
	for (Block* block : blockList)
	{
		D3DXVECTOR3 blockPosition = block->GetPosition();

		D3DXVECTOR3 direction = m_Position - blockPosition;
		float length = D3DXVec3Length(&direction);

		if (length < 2.3f)
		{
			scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);

			//enemy->SetDestroy();
			SetDestroy();
			return;
		}
	}
}

void EnemyBombSplash::Draw()
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