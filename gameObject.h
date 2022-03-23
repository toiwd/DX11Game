#pragma once

class Enemy;

class GameObject
{
protected://継承先のクラスからアクセスできる
	bool		m_Destroy = false;	//初期値falseに

	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Rotation;
	D3DXVECTOR3 m_Scale;

	Enemy* m_Pointer;

	// ジャンプの初速度
	float m_Jump_v0;

	// ジャンプの重力
	float m_Gravity;

	// キャラの速度
	D3DXVECTOR3 m_Speed;

public:
	GameObject() {}// コンストラクタ
	virtual ~GameObject() {}// デストラクタ(仮想関数)

	virtual void Init() = 0;//純粋仮想関数
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void SetPosition(D3DXVECTOR3 Position) { m_Position = Position; }
	void SetRotation(D3DXVECTOR3 Rotation) { m_Rotation = Rotation; }
	void SetScale(D3DXVECTOR3 Scale) { m_Scale = Scale; }
	void SetEnemy(Enemy* enemy) { m_Pointer = enemy; }

	void SetPositionScale(D3DXVECTOR3 Position, D3DXVECTOR3 Scale) { m_Position = Position; m_Scale = Scale; }
	void SetPositionRotation(D3DXVECTOR3 Position, D3DXVECTOR3 Rotation) { m_Position = Position; m_Rotation = Rotation; }

	D3DXVECTOR3 GetPosition(){ return m_Position; }
	D3DXVECTOR3 GetRotation(){ return m_Rotation; }
	D3DXVECTOR3 GetScale(){ return m_Scale; }

	D3DXVECTOR3 GetForward()// 前方向ベクトル取得
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot,
			m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	D3DXVECTOR3 GetUp()// 上方向ベクトル取得
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot,
			m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 Up;
		Up.x = rot._21;
		Up.y = rot._22;
		Up.z = rot._23;

		return Up;
	}

	D3DXVECTOR3 GetRight()// 右方向ベクトル取得
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot,
			m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 Right;
		Right.x = rot._11;
		Right.y = rot._12;
		Right.z = rot._13;

		return Right;
	}

	D3DXVECTOR3 GetObbX()
	{
		D3DXMATRIX rot, scale, world;
		D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
		world = scale * rot;

		D3DXVECTOR3 vx;
		vx.x = world._11;
		vx.y = world._12;
		vx.z = world._13;

		return vx * 1.2f;
	}

	D3DXVECTOR3 GetObbY()
	{
		D3DXMATRIX rot, scale, world;
		D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
		world = scale * rot;

		D3DXVECTOR3 vy;
		vy.x = world._21;
		vy.y = world._22;
		vy.z = world._23;

		return vy * 1.2f;
	}

	D3DXVECTOR3 GetObbZ()
	{
		D3DXMATRIX rot, scale, world;
		D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
		world = scale * rot;

		D3DXVECTOR3 vz;
		vz.x = world._31;
		vz.y = world._32;
		vz.z = world._33;

		return vz * 1.2f;
	}

	D3DXVECTOR3 GetObbX(float size)
	{
		D3DXMATRIX rot, scale, world;
		D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
		world = scale * rot;

		D3DXVECTOR3 vx;
		vx.x = world._11;
		vx.y = world._12;
		vx.z = world._13;

		return vx * size;
	}

	D3DXVECTOR3 GetObbY(float size)
	{
		D3DXMATRIX rot, scale, world;
		D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
		world = scale * rot;

		D3DXVECTOR3 vy;
		vy.x = world._21;
		vy.y = world._22;
		vy.z = world._23;

		return vy * size;
	}

	D3DXVECTOR3 GetObbZ(float size)
	{
		D3DXMATRIX rot, scale, world;
		D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
		world = scale * rot;

		D3DXVECTOR3 vz;
		vz.x = world._31;
		vz.y = world._32;
		vz.z = world._33;

		return vz * size;
	}

	void SetDestroy() { m_Destroy = true; }
	bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}
};