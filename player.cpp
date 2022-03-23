#include "main.h"
#include "manager.h"
#include "renderer.h"
#include <time.h>
#include "input.h"
#include "mouse.h"
#include "scene.h"
#include "option.h"
#include "model.h"
#include "player.h"
#include "bullet.h"
#include "secondBullet.h"
#include "camera.h"
#include "block.h"
#include "ink.h"
#include "enemyInk.h"
#include "bomb.h"
#include "enemyBombsplash.h"
#include "audio.h"
#include "animationModel.h"
#include "meshField.h"

#define JUMP (VK_SPACE)

Model* Player::m_ModelHuman;//スタティックメンバ変数はcppで再度宣言が必要
Model* Player::m_ModelSquid;

float Player::m_Sensi = 300.0f;

Mouse_State pMouse;

void Player::Load()
{
	m_ModelHuman = new Model();
	m_ModelSquid = new Model();
	m_ModelHuman->Load("asset\\model\\player.obj");
	m_ModelSquid->Load("asset\\model\\squid.obj");
}

void Player::Unload()
{
	m_ModelHuman->Unload();
	m_ModelSquid->Unload();
	delete m_ModelHuman;
	delete m_ModelSquid;
}

void Player::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 5.0f, -3.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.5f, 0.5f, 0.5f);

	m_Jump_v0 = 0.35f;
	m_Gravity = -0.015f;
	m_Speed = { 0.0f,0.0f,0.0f };
	m_JumpNow = false;

	m_PlayerState = false;
	m_OnInk = false;
	m_BulletRate = 0;
	m_Accel = 0.1f;
	m_InkTank = m_MaxInkTank;
	m_Life = m_MaxLife;
	m_DeadCool = 0;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");

	Scene* scene = Manager::GetScene();
	m_ShotSE = scene->AddGameObject<Audio>(1);
	m_ShotSE->Load("asset\\audio\\shoot.wav");
}

void Player::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Player::Update()
{
	// 相対座標モードに変更
	Mouse_SetMode(MOUSE_POSITION_MODE_RELATIVE);

	D3DXVECTOR3 forward = GetForward();
	D3DXVECTOR3 right = GetRight();
	
	Scene* scene = Manager::GetScene();

	D3DXVECTOR3 oldPosition = m_Position;
	
	// マウスの現在の状態を保持
	Mouse_GetState(&pMouse);

	if (Option::GetOptionEnd() == true)
	{
		pMouse.x = 0;
		pMouse.y = 0;
		Option::SetOptionEnd(false);
	}

	// 上下左右回転
	m_Rotation.y += (float)pMouse.x * 0.000025f * m_Sensi;
	m_Rotation.x += (float)pMouse.y * 0.000025f * m_Sensi;

	// 上に向きすぎないようにする
	if (m_Rotation.x < -3.14f / 2 + 0.2f)
	{
		m_Rotation.x = -3.14f / 2 + 0.2f;
	}
	// 下に向きすぎないようにする
	if (m_Rotation.x > 3.14f / 2 - 0.1f)
	{
		m_Rotation.x = 3.14f / 2 - 0.1f;
	}

	// 死亡した場合ここで操作を止める
	if (m_Dead == true)
	{
		m_DeadCool--;
		// クールタイムが終わったら体力を最大にし、ランダムな位置に移動
		if (m_DeadCool <= 0)
		{
			if (Input::GetKeyTrigger('R')|| m_DeadCool<=-300)
			{
				srand(time(NULL));
				int random = rand() % 5 + 1;
				m_Life = m_MaxLife;
				m_InkTank = m_MaxInkTank;
				switch (random)
				{
				case 1:
					m_Position.x = 0.0f;
					m_Position.z = 0.0f;
					break;
				case 2:
					m_Position.x = 0.0f;
					m_Position.z = 40.0f;
					break;
				case 3:
					m_Position.x = -40.0f;
					m_Position.z = 0.0f;
					break;
				case 4:
					m_Position.x = 40.0f;
					m_Position.z = 0.0f;
					break;
				case 5:
					m_Position.x = 0.0f;
					m_Position.z = -40.0f;
					break;
				default:
					break;
				}
				
				m_Dead = false;
			}
		}
		return;
	}

	// イカへ変更
	if (Input::GetKeyPress(VK_RBUTTON)||Input::GetKeyPress(VK_LSHIFT))
	{
		m_PlayerState = true;
		m_Scale = D3DXVECTOR3(0.3f, 0.3f, 0.3f);
		m_Gravity = -0.015f;// 重力の負荷が少し軽くなる
	}
	else
	{
		m_PlayerState = false;
		m_Scale = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
		m_Gravity = -0.015f;
	}

	Ink* ink = scene->GetGameObject<Ink>(1);
	MeshField* meshField = scene->GetGameObject<MeshField>(1);

	OnInk();
	OnEnemyInk();

	// インクの上且つイカ状態且つエネミーのインクの上ではない
	if (m_OnInk == true && m_PlayerState == true)
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
	else if (m_PlayerState == true)// イカ状態且つ地上
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
	if (m_OnEnemyInk == true && m_OnInk == false)
	{
		m_Accel = 0.01f;
	}

	std::vector<Block*> blockList = scene->GetGameObjects<Block>(1);

	

	// 移動
	if (Input::GetKeyPress('W'))
	{
		m_Position += forward * m_Accel;
	}
	if (Input::GetKeyPress('A'))
	{
		m_Position -= right * m_Accel;
	}
	if (Input::GetKeyPress('S'))
	{
		m_Position -= forward * m_Accel;
	}
	if (Input::GetKeyPress('D'))
	{
		m_Position += right * m_Accel;
	}

	// プレイヤーが地面にいるとき
	if (m_Position.y <= meshField->GetHeight(m_Position))
	{
		// ジャンプ
		if (Input::GetKeyTrigger(JUMP))
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


		// 上昇中にボタンを離したときの処理
		if (m_Speed.y > 0 && !Input::GetKeyPress(JUMP))
		{
			m_Speed.y *= 0.9f;
		}
	}
	
	// 地上の判定
	if (m_PlayerState == false || m_OnInk == false)
	{
		if (m_Position.y < meshField->GetHeight(m_Position))
		{
			m_Position.y = meshField->GetHeight(m_Position);
			m_JumpNow = false;
		}
	}

	// ブロックに当たると進まない
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
			m_Position = oldPosition;
		}
	}
	
	// 弾発射 ボム射出
	if (m_PlayerState == false)
	{
		if (m_BulletRate <= 0 && m_InkTank >= 1.0f)
		{
			if (Input::GetKeyPress(VK_LBUTTON))
			{
				Scene* scene = Manager::GetScene();

				scene->AddGameObject<SecondBullet>(1)->SetPosition(D3DXVECTOR3(m_Position.x, m_Position.y + 1.0f, m_Position.z));
				scene->AddGameObject<Bullet>(1)->SetPosition(D3DXVECTOR3(m_Position.x, m_Position.y + 1.0f, m_Position.z));

				m_BulletRate = 6;
				m_InkTank -= 1.0f;
				m_ShotSE->Play();
			}
		}
		if (m_InkTank >= 70.0f)
		{
			if (Input::GetKeyTrigger('Q'))
			{
				Scene* scene = Manager::GetScene();

				scene->AddGameObject<Bomb>(1)->SetPosition(D3DXVECTOR3(m_Position.x, m_Position.y + 1.0f, m_Position.z));

				m_InkTank -= 70.0f;
				m_ShotSE->Play();
			}
		}

	}
	m_BulletRate -= 1;

	// 体力がなくなるとインクを撒き散らして消滅
	if (m_Life <= 0.0f)
	{
		// 飛沫を飛ばす
		for (int i = 0; i < 30; i++)
		{
			Scene* scene = Manager::GetScene();
			scene->AddGameObject<EnemyBombSplash>(1)->SetPosition(D3DXVECTOR3(m_Position.x, 1.0f, m_Position.z));
		}
		m_Dead = true;
		m_DeadCool = 100;
	}
}

void Player::Draw()
{
	// 視錐台カリング
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>(0);

	if (!camera->CheckView(m_Position))
		return;

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);

	// ピッチだけ0.0fに固定する
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, 0.0f, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	// 死亡時は描画しない
	if (m_Dead == false)
	{
		if (m_PlayerState == false)
		{
			m_ModelHuman->Draw();
		}
		else
		{
			m_ModelSquid->Draw();
		}
	}
}

void Player::OnInk()
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
			m_OnInk = true;
			return;
		}
		else
		{
			m_OnInk = false;
		}
	}
}

void Player::OnEnemyInk()
{
	Scene* scene = Manager::GetScene();
	std::vector<EnemyInk*> inkList = scene->GetGameObjects<EnemyInk>(1);

	// インクの上判定
	for (EnemyInk* ink : inkList)
	{
		D3DXVECTOR3 inkPosition = ink->GetPosition();

		D3DXVECTOR3 direction = m_Position - inkPosition;
		float length = D3DXVec3Length(&direction);

		if (length < 1.2f)
		{
			m_OnEnemyInk = true;
			return;
		}
		else
		{
			m_OnEnemyInk = false;
		}
	}
}
