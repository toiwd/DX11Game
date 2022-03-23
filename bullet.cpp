#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "model.h"
#include "bullet.h"
#include "player.h"
#include "enemy.h"
#include "block.h"
#include "ink.h"
#include "explosion.h"
#include "meshField.h"
#include "enemyInk.h"

Model* Bullet::m_Model;//スタティックメンバ変数はcppで再度宣言が必要

#define BULLET_JUMP (0.10f)	// 弾の飛ぶ高さ

void Bullet::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\bullet.obj");
}

void Bullet::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void Bullet::Init()
{
	m_Position = D3DXVECTOR3(-2.0f, 1.5f, -1.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.1f, 0.1f, 0.1f);

	m_Jump_v0 = BULLET_JUMP;
	m_Gravity = -0.015f;
	m_range = 0.6f;
	m_Speed = { 0.0f,0.0f,0.0f };
	
	// 初速度の設定
	m_Speed.y = m_Jump_v0;
	m_Speed.x = m_range;

	// プレイヤーの向きを反映させる
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(1);
	D3DXVECTOR3 playerRotation = player->GetRotation();
	m_Rotation = playerRotation;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");
}

void Bullet::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Bullet::Update()
{
	Scene* scene = Manager::GetScene();
	std::vector<Enemy*> enemyList = scene->GetGameObjects<Enemy>(1);
	std::vector<Block*> blockList = scene->GetGameObjects<Block>(1);
	std::vector<Ink*> inkList = scene->GetGameObjects<Ink>(1);
	MeshField* meshField = scene->GetGameObject<MeshField>(1);
	D3DXVECTOR3 forward = GetForward();

	// 正面に向かって飛んでいく
	m_Position += forward * m_Speed.x;

	// 重力
	// Y方向の速度に加速度を加える
	m_Speed.y += m_Gravity;
	m_Speed.x += m_Gravity;
	// Y座標の更新
	m_Position.y += m_Speed.y;

	
	// 地面に落ちたら弾を消す
	if (m_Position.y < meshField->GetHeight(m_Position))
	{
		// 色を塗る
		scene->AddGameObject<Ink>(1)->SetPosition(D3DXVECTOR3(m_Position.x, m_Position.y+0.2f, m_Position.z));
		//Ink::Paint(m_Position);
		
		std::vector<EnemyInk*> inkList = scene->GetGameObjects<EnemyInk>(1);

		// 近くにエネミーのインクがあれば消す
		for (EnemyInk* ink : inkList)
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
				SetDestroy();	// 消す予約
				return;
			}
		}

		SetDestroy();	// 消す予約
		return;
	}

	// obbのYができたらこっちを使う
	for (Enemy* enemy : enemyList)
	{
		D3DXVECTOR3 enemyPosition = enemy->GetPosition();
		D3DXVECTOR3 direction = m_Position - enemyPosition;
		bool enemyState = enemy->GetEnemyState();

		D3DXVECTOR3 obbx, obby, obbz;
		float obbLenx, obbLeny, obbLenz;

		// エネミーの状態によって当たり判定を変える
		if (enemyState == false)
		{
			obbx = enemy->GetObbX(1.2f);
			obbLenx = D3DXVec3Length(&obbx);
			obbx /= obbLenx;

			obby = enemy->GetObbY(2.4f);
			obbLeny = D3DXVec3Length(&obby);
			obby /= obbLeny;

			obbz = enemy->GetObbZ(1.2f);
			obbLenz = D3DXVec3Length(&obbz);
			obbz /= obbLenz;
		}
		else
		{
			obbx = enemy->GetObbX(2.5f);
			obbLenx = D3DXVec3Length(&obbx);
			obbx /= obbLenx;

			obby = enemy->GetObbY(2.5f);
			obbLeny = D3DXVec3Length(&obby);
			obby /= obbLeny;

			obbz = enemy->GetObbZ(2.5f);
			obbLenz = D3DXVec3Length(&obbz);
			obbz /= obbLenz;
		}

		float lenX, lenY, lenZ;
		lenX = D3DXVec3Dot(&obbx, &direction);
		lenY = D3DXVec3Dot(&obby, &direction);
		lenZ = D3DXVec3Dot(&obbz, &direction);

		if (fabs(lenX) < obbLenx && fabs(lenY) < obbLeny && fabs(lenZ) < obbLenz)
		{
			scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);

			enemy->m_Life -= 10.0f;

			SetDestroy();
			return;
		}
	}

	// ブロックに当たると弾だけ消える
	for (Block* block : blockList)
	{
		D3DXVECTOR3 blockPosition = block->GetPosition();
		D3DXVECTOR3 direction = m_Position - blockPosition;

		D3DXVECTOR3 obbx, obby, obbz;
		float obbLenx, obbLeny, obbLenz;

		obbx = block->GetObbX(1.2f);
		obbLenx = D3DXVec3Length(&obbx);
		obbx /= obbLenx;

		obby = block->GetObbY(1.2f);
		obbLeny = D3DXVec3Length(&obby);
		obby /= obbLeny;

		obbz = block->GetObbZ(1.2f);
		obbLenz = D3DXVec3Length(&obbz);
		obbz /= obbLenz;

		float lenX, lenY, lenZ;
		lenX = D3DXVec3Dot(&obbx, &direction);
		lenY = D3DXVec3Dot(&obby, &direction);
		lenZ = D3DXVec3Dot(&obbz, &direction);

		if (fabs(lenX) < obbLenx && fabs(lenY) < obbLeny && fabs(lenZ) < obbLenz)
		{
			scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);

			//enemy->SetDestroy();
			SetDestroy();
			return;
		}
	}
}

void Bullet::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();
}