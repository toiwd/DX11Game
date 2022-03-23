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

Model* Enemy::m_ModelHuman;//スタティックメンバ変数はcppで再度宣言が必要
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

	

	
	
	
	
	// エネミーのイカ状態
	if (m_EnemyState == true)
	{
		m_Scale = D3DXVECTOR3(0.3f, 0.3f, 0.3f);
		m_Gravity = -0.015f;// 重力の負荷が少し軽くなる
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

	// インクの上且つイカ状態
	if (m_OnInk == true && m_EnemyState == true)
	{
		// インクの上でイカ状態になると残量が増える
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
	else if (m_EnemyState == true)// イカ状態且つ地上
	{
		if (m_JumpNow == false)// プレイヤーがジャンプ中でない場合
		{
			m_Accel = 0.05f;
		}

	}
	else
	{
		m_Accel = 0.1f;
	}

	// 敵のインクの上且つ自分のインクの上でない
	if (m_OnPlayerInk == true && m_OnInk == false)
	{
		m_Accel = 0.01f;
	}

	// プレイヤーが地面にいるとき
	if (m_Position.y <= meshField->GetHeight(m_Position))
	{
		// ジャンプ
		if (m_Jump == true && m_JumpNow == false)
		{
			// 沈んでいても座標を地上に戻す
			m_Position.y = meshField->GetHeight(m_Position);
			// 初速度の設定
			m_Speed.y = m_Jump_v0;
			// Y座標の更新
			m_Position.y += m_Speed.y;
			// ジャンプ中の判定
			m_JumpNow = true;
		}
	}
	else
	{
		// 重力
		// Y方向の速度に加速度を加える
		m_Speed.y += m_Gravity;

		// Y座標の更新
		m_Position.y += m_Speed.y;
	}

	// 地上の判定
	if (m_EnemyState == false || m_OnInk == false)
	{
		if (m_Position.y < meshField->GetHeight(m_Position))
		{
			m_Position.y = meshField->GetHeight(m_Position);
			m_JumpNow = false;
		}

	}

	// 体力がなくなるとインクを撒き散らして消滅
	if (m_Life <= 0.0f)
	{
		// 飛沫を飛ばす
		for (int i = 0; i < 30; i++)
		{
			Scene* scene = Manager::GetScene();
			scene->AddGameObject<BombSplash>(1)->SetPosition(D3DXVECTOR3(m_Position.x, 1.0f, m_Position.z));
		}
		SetDestroy();

		return;
	}

	// プレイヤーを倒したら見失う
	if (player->m_Life <= 0)
	{
		m_Found = false;
	}

	m_BulletRate -= 1;
}

void Enemy::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);

	// ピッチだけ0にする
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
	// イカ状態だと弾が打てない
	if (m_EnemyState == false)
	{
		// 一定間隔で弾を飛ばす
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

/* AIによる行動 */
// ショット攻撃
void Enemy::ShotAttack()
{
	m_EnemyState = false;

	Scene* scene = Manager::GetScene();
	Player* player = Manager::GetScene()->GetGameObject<Player>(1);
	D3DXVECTOR3 playerPosition = player->GetPosition();

	// 内積
	float EnemyAngleX = Enemy::Angle(m_Position, playerPosition);
	float EnemyAngleY = Enemy::Angle(m_Position.z, m_Position.x, playerPosition.z, playerPosition.x);

	EnemyAngleY -= 180.0f;

	// 算出された角度をRotationの値に変換する
	m_Rotation.x = PI * 2 * EnemyAngleX / 360.0f;
	m_Rotation.y = PI * 2 * EnemyAngleY / 360.0f;

	// 弾発射
	Shot();

}

// ボム使用
void Enemy::UseBomb()
{
	m_EnemyState = false;

	Scene* scene = Manager::GetScene();
	Player* player = Manager::GetScene()->GetGameObject<Player>(1);
	D3DXVECTOR3 playerPosition = player->GetPosition();

	// 内積
	float enemyAngleY = Enemy::Angle(m_Position.z, m_Position.x, playerPosition.z, playerPosition.x);

	// 後ろ向いていたので正面に戻す
	enemyAngleY -= 180.0f;

	// 算出された角度をRotationの値に変換する
	m_Rotation.x = PI * 2 * 45.0f / 360.0f;
	m_Rotation.y = PI * 2 * enemyAngleY / 360.0f;

	// ボム射出
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

// 潜伏（インクの回復と体力の回復）
void Enemy::Dive()
{
	m_EnemyState = true;
}

// 前方を塗る
void Enemy::ForwardPaint()
{
	m_EnemyState = false;

	Scene* scene = Manager::GetScene();
	Player* player = Manager::GetScene()->GetGameObject<Player>(1);
	D3DXVECTOR3 playerPosition = player->GetPosition();

	// 内積
	float enemyAngleY = Enemy::Angle(m_Position.z, m_Position.x, playerPosition.z, playerPosition.x);

	// 後ろ向いていたので正面に戻す
	enemyAngleY -= 180.0f;

	// 算出された角度をRotationの値に変換する
	// 前方をきれいに塗るため少し前後に塗る
	float random = (float)rand() / RAND_MAX * 60.0f - 20.0f;
	m_Rotation.x = PI * 2 * random / 360.0f;
	m_Rotation.y = PI * 2 * enemyAngleY / 360.0f;

	// 弾発射
	// イカ状態だと弾が打てない
	if (m_EnemyState == false)
	{
		// 一定間隔で弾を飛ばす
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

// 追いかけるために塗る
void Enemy::ChasePaint()
{
	m_EnemyState = false;

	// 前方をきれいに塗るため少し前後に塗る
	float random = (float)rand() / RAND_MAX * 60.0f - 20.0f;
	m_Rotation.x = PI * 2 * random / 360.0f;

	// 弾発射
	// イカ状態だと弾が打てない
	if (m_EnemyState == false)
	{
		// 一定間隔で弾を飛ばす
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

// 自分の下を塗る
void Enemy::UnderPaint()
{
	m_EnemyState = false;

	Scene* scene = Manager::GetScene();
	Player* player = Manager::GetScene()->GetGameObject<Player>(1);
	D3DXVECTOR3 playerPosition = player->GetPosition();

	// 内積
	float enemyAngleY = Enemy::Angle(m_Position.z, m_Position.x, playerPosition.z, playerPosition.x);

	// 後ろ向いていたので正面に戻す
	enemyAngleY -= 180.0f;

	// 算出された角度をRotationの値に変換する
	m_Rotation.x = PI * 2 * 90.0f / 360.0f;
	m_Rotation.y = PI * 2 * enemyAngleY / 360.0f;

	// 弾発射
	Shot();
}

// プレイヤー追跡
void Enemy::WalkChasePlayer()
{
	// この距離より遠ければ、見失う
	if (IsDistanceOver(m_FoundDistance))
	{
		m_Found = false;
	}
	
	m_EnemyState = false;

	Scene* scene = Manager::GetScene();
	Player* player = Manager::GetScene()->GetGameObject<Player>(1);
	D3DXVECTOR3 playerPosition = player->GetPosition();

	D3DXVECTOR3 forward = GetForward();
	// 内積
	float enemyAngleX = Enemy::Angle(m_Position, playerPosition);
	float enemyAngleY = Enemy::Angle(m_Position.z, m_Position.x, playerPosition.z, playerPosition.x);

	// 後ろ向いていたので正面に戻す
	enemyAngleY -= 180.0f;

	// 算出された角度をRotationの値に変換する
	m_Rotation.x = PI * 2 * 0.0f / 360.0f;
	m_Rotation.y = PI * 2 * enemyAngleY / 360.0f;

	// 前に進む
	m_Position += forward * m_Accel;

}

// 経路探索を用いて追いかける
void Enemy::Pathfinder()
{
	// この距離より遠ければ、見失う
	if (IsDistanceOver(m_FoundDistance))
	{
		m_Found = false;
	}

	m_EnemyState = false;

	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(1);
	D3DXVECTOR3 playerPosition = player->GetPosition();

	D3DXVECTOR3 aStarPosition;

	// リストの中身がなくなるまで
	if (GetList().empty() == false)
	{
		Cell routeAStar = GetList().front();

		aStarPosition.x = ((float)routeAStar.X - 10) * 5.0f + 2.5f;
		aStarPosition.z = ((float)routeAStar.Z - 10) * -5.0f - 2.5f;

		D3DXVECTOR3 forward = GetForward();
		// 内積
		float enemyAngleY = Enemy::Angle(m_Position.z, m_Position.x, aStarPosition.z, aStarPosition.x);

		// 後ろ向いていたので正面に戻す
		enemyAngleY -= 180.0f;

		// 算出された角度をRotationの値に変換する
		m_Rotation.x = PI * 2 * 0.0f / 360.0f;
		m_Rotation.y = PI * 2 * enemyAngleY / 360.0f;

		// 前に進む
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
	// この距離より遠ければ、見失う
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
	// インクの上判定
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
	// インクの上判定
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

// 2点の角度を返す
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

// 2点間の距離を返す
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
	// 指定した距離よりも遠いかどうか
	if (Distance(m_Position.x, m_Position.z, playerPosition.x, playerPosition.z) >= check_distance)
	{
		return true;
	}

	return false;
}
