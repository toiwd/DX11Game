#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "model.h"
#include "player.h"
#include "enemy.h"
#include "enemyBullet.h"
#include "enemySecondBullet.h"
#include "bullet.h"
#include "enemyBomb.h"
#include "bombSplash.h"
#include "ink.h"
#include "enemyInk.h"
#include "enemyHpBar.h"
#include "meshField.h"
#include "behaviorTree.h"
#include "behaviorData.h"
#include "attackJudgment.h"
#include "shotAction.h"

#include "aStar.h"

#define PI 3.14159265359

Model* Enemy::m_ModelHuman;//�X�^�e�B�b�N�����o�ϐ���cpp�ōēx�錾���K�v
Model* Enemy::m_ModelSquid;

Enemy::Enemy(BehaviorTree *ai_tree):
	m_AiTree(ai_tree),
	m_BehaviorData(NULL),
	m_ActiveNode(NULL)
{
	m_BehaviorData = new BehaviorData();
}

void Enemy::Load()
{
	m_ModelHuman = new Model();
	m_ModelSquid = new Model();
	m_ModelHuman->Load("asset\\model\\enemyblue.obj");
	m_ModelSquid->Load("asset\\model\\squid.obj");
}

void Enemy::Unload()
{
	m_ModelHuman->Unload();
	m_ModelSquid->Unload();
	delete m_ModelHuman;
	delete m_ModelSquid;
}


void Enemy::Init()
{
	m_Position = D3DXVECTOR3(2.0f, 1.0f, -1.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.5f, 0.5f, 0.5f);

	m_Jump_v0 = 0.35f;
	m_Gravity = -0.015f;
	m_Speed = { 0.0f,0.0f,0.0f };

	m_JumpNow = false;
	m_Jump = false;

	m_EnemyState = false;
	m_OnInk = false;
	m_Found = false;
	m_BulletRate = 0;
	m_Accel = 0.1f;
	m_InkTank = 100.0f;
	m_ShotCount = 0;

	m_Life = 100.0f;

	Scene* scene = Manager::GetScene();
	scene->AddGameObject<EnemyHpBar>(1)->SetEnemy(this);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");
}

void Enemy::Uninit()
{

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}


void Enemy::Update()
{
	
	if (m_ActiveNode == NULL)
	{
		m_ActiveNode = m_AiTree->Inference(this, m_BehaviorData);
	}
	if (m_ActiveNode != NULL)
	{
		m_ActiveNode = m_AiTree->Run(this, m_ActiveNode, m_BehaviorData);
	}
	
	
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(1);
	D3DXVECTOR3 playerPosition = player->GetPosition();

	

	
	
	
	
	// �G�l�~�[�̃C�J���
	if (m_EnemyState == true)
	{
		m_Scale = D3DXVECTOR3(0.3f, 0.3f, 0.3f);
		m_Gravity = -0.015f;// �d�͂̕��ׂ������y���Ȃ�
	}
	else
	{
		m_Scale = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
		m_Gravity = -0.015f;
	}

	EnemyInk* ink = scene->GetGameObject<EnemyInk>(1);
	MeshField* meshField = scene->GetGameObject<MeshField>(1);

	OnInk();

	OnPlayerInk();

	// �C���N�̏㊎�C�J���
	if (m_OnInk == true && m_EnemyState == true)
	{
		// �C���N�̏�ŃC�J��ԂɂȂ�Ǝc�ʂ�������
		m_InkTank += 0.5f;
		m_Life += 0.5f;
		if (m_InkTank >= m_MaxInkTank)
		{
			m_InkTank = m_MaxInkTank;
		}
		if (m_Life >= m_MaxLife)
		{
			m_Life = m_MaxLife;
		}

		m_Accel = 0.2f;

		if (m_Speed.y < 0)
		{
			m_Position.y = meshField->GetHeight(m_Position) - 0.5f;
		}

	}
	else if (m_EnemyState == true)// �C�J��Ԋ��n��
	{
		if (m_JumpNow == false)// �v���C���[���W�����v���łȂ��ꍇ
		{
			m_Accel = 0.05f;
		}

	}
	else
	{
		m_Accel = 0.1f;
	}

	// �G�̃C���N�̏㊎�����̃C���N�̏�łȂ�
	if (m_OnPlayerInk == true && m_OnInk == false)
	{
		m_Accel = 0.01f;
	}

	// �v���C���[���n�ʂɂ���Ƃ�
	if (m_Position.y <= meshField->GetHeight(m_Position))
	{
		// �W�����v
		if (m_Jump == true && m_JumpNow == false)
		{
			// ����ł��Ă����W��n��ɖ߂�
			m_Position.y = meshField->GetHeight(m_Position);
			// �����x�̐ݒ�
			m_Speed.y = m_Jump_v0;
			// Y���W�̍X�V
			m_Position.y += m_Speed.y;
			// �W�����v���̔���
			m_JumpNow = true;
		}
	}
	else
	{
		// �d��
		// Y�����̑��x�ɉ����x��������
		m_Speed.y += m_Gravity;

		// Y���W�̍X�V
		m_Position.y += m_Speed.y;
	}

	// �n��̔���
	if (m_EnemyState == false || m_OnInk == false)
	{
		if (m_Position.y < meshField->GetHeight(m_Position))
		{
			m_Position.y = meshField->GetHeight(m_Position);
			m_JumpNow = false;
		}

	}

	// �̗͂��Ȃ��Ȃ�ƃC���N���T���U�炵�ď���
	if (m_Life <= 0.0f)
	{
		// �򖗂��΂�
		for (int i = 0; i < 30; i++)
		{
			Scene* scene = Manager::GetScene();
			scene->AddGameObject<BombSplash>(1)->SetPosition(D3DXVECTOR3(m_Position.x, 1.0f, m_Position.z));
		}
		SetDestroy();

		return;
	}

	// �v���C���[��|�����猩����
	if (player->m_Life <= 0)
	{
		m_Found = false;
	}

	m_BulletRate -= 1;
}

void Enemy::Draw()
{
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);

	// �s�b�`����0�ɂ���
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, 0.0f, 0.0f);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	if (m_EnemyState == false)
	{
		m_ModelHuman->Draw();
	}
	else
	{
		m_ModelSquid->Draw();
	}
}

void Enemy::Shot()
{
	// �C�J��Ԃ��ƒe���łĂȂ�
	if (m_EnemyState == false)
	{
		// ���Ԋu�Œe���΂�
		if (m_BulletRate <= 0 && m_InkTank >= 1.0f)
		{
			Scene* scene = Manager::GetScene();
			scene->AddGameObject<EnemyBullet>(1)->SetPositionRotation(D3DXVECTOR3(m_Position.x, m_Position.y + 1.0f, m_Position.z), D3DXVECTOR3(m_Rotation));
			scene->AddGameObject<EnemySecondBullet>(1)->SetPositionRotation(D3DXVECTOR3(m_Position.x, m_Position.y + 1.0f, m_Position.z), D3DXVECTOR3(m_Rotation));

			m_BulletRate = 6;
			m_InkTank -= 1.0f;
		}
	}
}

/* AI�ɂ��s�� */
// �V���b�g�U��
void Enemy::ShotAttack()
{
	m_EnemyState = false;

	Scene* scene = Manager::GetScene();
	Player* player = Manager::GetScene()->GetGameObject<Player>(1);
	D3DXVECTOR3 playerPosition = player->GetPosition();

	// ����
	float EnemyAngleX = Enemy::Angle(m_Position, playerPosition);
	float EnemyAngleY = Enemy::Angle(m_Position.z, m_Position.x, playerPosition.z, playerPosition.x);

	EnemyAngleY -= 180.0f;

	// �Z�o���ꂽ�p�x��Rotation�̒l�ɕϊ�����
	m_Rotation.x = PI * 2 * EnemyAngleX / 360.0f;
	m_Rotation.y = PI * 2 * EnemyAngleY / 360.0f;

	// �e����
	Shot();

}

// �{���g�p
void Enemy::UseBomb()
{
	m_EnemyState = false;

	Scene* scene = Manager::GetScene();
	Player* player = Manager::GetScene()->GetGameObject<Player>(1);
	D3DXVECTOR3 playerPosition = player->GetPosition();

	// ����
	float enemyAngleY = Enemy::Angle(m_Position.z, m_Position.x, playerPosition.z, playerPosition.x);

	// �������Ă����̂Ő��ʂɖ߂�
	enemyAngleY -= 180.0f;

	// �Z�o���ꂽ�p�x��Rotation�̒l�ɕϊ�����
	m_Rotation.x = PI * 2 * 45.0f / 360.0f;
	m_Rotation.y = PI * 2 * enemyAngleY / 360.0f;

	// �{���ˏo
	if (m_EnemyState == false)
	{
		if (m_BulletRate <= 0 && m_InkTank >= 70.0f)
		{
			Scene* scene = Manager::GetScene();

			scene->AddGameObject<EnemyBomb>(1)->SetPositionRotation(D3DXVECTOR3(m_Position.x, m_Position.y + 1.0f, m_Position.z), D3DXVECTOR3(m_Rotation));

			m_InkTank -= 70.0f;
			m_BulletRate = 6;
		}

	}
}

// �����i�C���N�̉񕜂Ƒ̗͂̉񕜁j
void Enemy::Dive()
{
	m_EnemyState = true;
}

// �O����h��
void Enemy::ForwardPaint()
{
	m_EnemyState = false;

	Scene* scene = Manager::GetScene();
	Player* player = Manager::GetScene()->GetGameObject<Player>(1);
	D3DXVECTOR3 playerPosition = player->GetPosition();

	// ����
	float enemyAngleY = Enemy::Angle(m_Position.z, m_Position.x, playerPosition.z, playerPosition.x);

	// �������Ă����̂Ő��ʂɖ߂�
	enemyAngleY -= 180.0f;

	// �Z�o���ꂽ�p�x��Rotation�̒l�ɕϊ�����
	// �O�������ꂢ�ɓh�邽�ߏ����O��ɓh��
	float random = (float)rand() / RAND_MAX * 60.0f - 20.0f;
	m_Rotation.x = PI * 2 * random / 360.0f;
	m_Rotation.y = PI * 2 * enemyAngleY / 360.0f;

	// �e����
	// �C�J��Ԃ��ƒe���łĂȂ�
	if (m_EnemyState == false)
	{
		// ���Ԋu�Œe���΂�
		if (m_BulletRate <= 0 && m_InkTank >= 1.0f)
		{
			Scene* scene = Manager::GetScene();
			scene->AddGameObject<EnemyBullet>(1)->SetPositionRotation(D3DXVECTOR3(m_Position.x, m_Position.y + 1.0f, m_Position.z), D3DXVECTOR3(m_Rotation));
			scene->AddGameObject<EnemySecondBullet>(1)->SetPositionRotation(D3DXVECTOR3(m_Position.x, m_Position.y + 1.0f, m_Position.z), D3DXVECTOR3(m_Rotation));

			m_BulletRate = 6;
			m_InkTank -= 1.0f;
			m_ShotCount++;
		}
	}
}

// �ǂ������邽�߂ɓh��
void Enemy::ChasePaint()
{
	m_EnemyState = false;

	// �O�������ꂢ�ɓh�邽�ߏ����O��ɓh��
	float random = (float)rand() / RAND_MAX * 60.0f - 20.0f;
	m_Rotation.x = PI * 2 * random / 360.0f;

	// �e����
	// �C�J��Ԃ��ƒe���łĂȂ�
	if (m_EnemyState == false)
	{
		// ���Ԋu�Œe���΂�
		if (m_BulletRate <= 0 && m_InkTank >= 1.0f)
		{
			Scene* scene = Manager::GetScene();
			scene->AddGameObject<EnemyBullet>(1)->SetPositionRotation(D3DXVECTOR3(m_Position.x, m_Position.y + 1.0f, m_Position.z), D3DXVECTOR3(m_Rotation));
			scene->AddGameObject<EnemySecondBullet>(1)->SetPositionRotation(D3DXVECTOR3(m_Position.x, m_Position.y + 1.0f, m_Position.z), D3DXVECTOR3(m_Rotation));

			m_BulletRate = 6;
			m_InkTank -= 1.0f;
			m_ShotCount++;
		}
	}
}

// �����̉���h��
void Enemy::UnderPaint()
{
	m_EnemyState = false;

	Scene* scene = Manager::GetScene();
	Player* player = Manager::GetScene()->GetGameObject<Player>(1);
	D3DXVECTOR3 playerPosition = player->GetPosition();

	// ����
	float enemyAngleY = Enemy::Angle(m_Position.z, m_Position.x, playerPosition.z, playerPosition.x);

	// �������Ă����̂Ő��ʂɖ߂�
	enemyAngleY -= 180.0f;

	// �Z�o���ꂽ�p�x��Rotation�̒l�ɕϊ�����
	m_Rotation.x = PI * 2 * 90.0f / 360.0f;
	m_Rotation.y = PI * 2 * enemyAngleY / 360.0f;

	// �e����
	Shot();
}

// �v���C���[�ǐ�
void Enemy::WalkChasePlayer()
{
	// ���̋�����艓����΁A������
	if (IsDistanceOver(m_FoundDistance))
	{
		m_Found = false;
	}
	
	m_EnemyState = false;

	Scene* scene = Manager::GetScene();
	Player* player = Manager::GetScene()->GetGameObject<Player>(1);
	D3DXVECTOR3 playerPosition = player->GetPosition();

	D3DXVECTOR3 forward = GetForward();
	// ����
	float enemyAngleX = Enemy::Angle(m_Position, playerPosition);
	float enemyAngleY = Enemy::Angle(m_Position.z, m_Position.x, playerPosition.z, playerPosition.x);

	// �������Ă����̂Ő��ʂɖ߂�
	enemyAngleY -= 180.0f;

	// �Z�o���ꂽ�p�x��Rotation�̒l�ɕϊ�����
	m_Rotation.x = PI * 2 * 0.0f / 360.0f;
	m_Rotation.y = PI * 2 * enemyAngleY / 360.0f;

	// �O�ɐi��
	m_Position += forward * m_Accel;

}

// �o�H�T����p���Ēǂ�������
void Enemy::Pathfinder()
{
	// ���̋�����艓����΁A������
	if (IsDistanceOver(m_FoundDistance))
	{
		m_Found = false;
	}

	m_EnemyState = false;

	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(1);
	D3DXVECTOR3 playerPosition = player->GetPosition();

	D3DXVECTOR3 aStarPosition;

	// ���X�g�̒��g���Ȃ��Ȃ�܂�
	if (GetList().empty() == false)
	{
		Cell routeAStar = GetList().front();

		aStarPosition.x = ((float)routeAStar.X - 10) * 5.0f + 2.5f;
		aStarPosition.z = ((float)routeAStar.Z - 10) * -5.0f - 2.5f;

		D3DXVECTOR3 forward = GetForward();
		// ����
		float enemyAngleY = Enemy::Angle(m_Position.z, m_Position.x, aStarPosition.z, aStarPosition.x);

		// �������Ă����̂Ő��ʂɖ߂�
		enemyAngleY -= 180.0f;

		// �Z�o���ꂽ�p�x��Rotation�̒l�ɕϊ�����
		m_Rotation.x = PI * 2 * 0.0f / 360.0f;
		m_Rotation.y = PI * 2 * enemyAngleY / 360.0f;

		// �O�ɐi��
		m_Position += forward * m_Accel;

		if (Enemy::Distance(m_Position.x, m_Position.z, aStarPosition.x, aStarPosition.z) <= 0.1f)
		{
			PopList();
			AStar(m_Position.x, m_Position.z, playerPosition.x, playerPosition.z);
		}
	}
	else
	{
		AStar(m_Position.x, m_Position.z, playerPosition.x, playerPosition.z);
	}
}

void Enemy::Wait()
{
	// ���̋�����艓����΁A������
	if (IsDistanceOver(m_FoundDistance))
	{
		m_Found = false;
	}
	else
	{
		m_Found = true;
	}

}

void Enemy::OnInk()
{
	Scene* scene = Manager::GetScene();
	std::vector<EnemyInk*> inkList = scene->GetGameObjects<EnemyInk>(1);
	// �C���N�̏㔻��
	for (EnemyInk* ink : inkList)
	{
		D3DXVECTOR3 inkPosition = ink->GetPosition();

		D3DXVECTOR3 direction = m_Position - inkPosition;
		float length = D3DXVec3Length(&direction);

		if (length < 1.3f)
		{
			m_OnInk = true;
			return;
		}
		else
		{
			m_OnInk = false;
		}
	}
}

void Enemy::OnPlayerInk()
{
	Scene* scene = Manager::GetScene();
	std::vector<Ink*> inkList = scene->GetGameObjects<Ink>(1);
	// �C���N�̏㔻��
	for (Ink* ink : inkList)
	{
		D3DXVECTOR3 inkPosition = ink->GetPosition();

		D3DXVECTOR3 direction = m_Position - inkPosition;
		float length = D3DXVec3Length(&direction);

		if (length < 1.2f)
		{
			m_OnPlayerInk = true;
			return;
		}
		else
		{
			m_OnPlayerInk = false;
		}
	}
}

// 2�_�̊p�x��Ԃ�
float Enemy::Angle(float E1, float E2, float P1, float P2)
{
	float r = atan2f(E2 - P2, E1 - P1);

	if (r < 0)
	{
		r = r + 2 * PI;
	}

	return (float)floor(r * 360 / (2 * PI));
}

float Enemy::Angle(D3DXVECTOR3 enemy_Position, D3DXVECTOR3 player_Position)
{
	float r = atan2f(enemy_Position.y - player_Position.y,
		sqrtf((enemy_Position.x - player_Position.x) * (enemy_Position.x - player_Position.x) + (enemy_Position.z - player_Position.z)*(enemy_Position.z - player_Position.z)));

	if (r < 0)
	{
		r = r + 2 * PI;
	}

	return (float)floor(r * 360 / (2 * PI));
}

// 2�_�Ԃ̋�����Ԃ�
float Enemy::Distance(float Ex, float Ez, float Px, float Pz)
{
	float distance = sqrtf((Px - Ex)*(Px - Ex) + (Pz - Ez)*(Pz - Ez));

	return distance;
}

bool Enemy::IsHpPercentOver(float check_percent)
{
	float percent = m_Life / m_MaxLife;

	if (percent * 100.0f >= check_percent)
	{
		return true;
	}

	return false;
}

bool Enemy::IsInkPercentOver(float check_percent)
{
	float percent = m_InkTank / m_MaxInkTank;

	if (percent * 100.0f >= check_percent)
	{
		return true;
	}

	return false;
}

bool Enemy::IsDistanceOver(float check_distance)
{
	Scene* scene = Manager::GetScene();
	Player* player = Manager::GetScene()->GetGameObject<Player>(1);
	D3DXVECTOR3 playerPosition = player->GetPosition();
	// �w�肵�����������������ǂ���
	if (Distance(m_Position.x, m_Position.z, playerPosition.x, playerPosition.z) >= check_distance)
	{
		return true;
	}

	return false;
}
