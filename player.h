#pragma once

#include "gameObject.h"

class Player : public GameObject
{

private:
	static class Model* m_ModelHuman;
	static class Model* m_ModelSquid;

	// プレイヤーの状態
	bool m_PlayerState;
	// プレイヤーがインクの上か
	bool m_OnInk;
	// プレイヤーがエネミーのインクの上か
	bool m_OnEnemyInk;
	// プレイヤーがジャンプ中か
	bool m_JumpNow;
	// 弾の発射レート
	int m_BulletRate;
	// プレイヤーの加速
	float m_Accel;
	// プレイヤーのマウス感度
	static float m_Sensi;
	// プレイヤーの生死判定
	bool m_Dead;
	// 死亡後のクールタイム
	int m_DeadCool;

	ID3D11VertexShader*		m_VertexShader = NULL;
	ID3D11PixelShader*		m_PixelShader = NULL;
	ID3D11InputLayout*		m_VertexLayout = NULL;

	class Audio* m_ShotSE;

public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void OnInk();
	void OnEnemyInk();

	// キャラの体力
	const float m_MaxLife = 100.0f;
	float m_Life;
	// 弾の残量
	const float m_MaxInkTank = 100.0f;
	float m_InkTank;

	// プレイヤーの状態取得
	bool GetPlayerState() { return m_PlayerState; }
	bool GetOnInk() { return m_OnInk; }
	bool GetOnEnemyInk() { return m_OnEnemyInk; }
	bool GetDead() { return m_Dead; }
	float GetLife() { return m_Life; }

	// プレイヤーの弾の残量取得
	float GetInkTank() { return m_InkTank; }

	// プレイヤーのマウス感度設定
	static void SetPlayerSens(float Sens)
	{
		if (m_Sensi == Sens) return;
		m_Sensi = Sens;
	}
	
};