#pragma once

#include "gameObject.h"

class BehaviorTree;
class BehaviorData;
class NodeBase;
class GameObject;

class Enemy : public GameObject
{

private:
	static class Model* m_ModelHuman;
	static class Model* m_ModelSquid;

	// エネミーの状態
	bool m_EnemyState=false;
	// エネミーがインクの上か
	bool m_OnInk;
	// エネミーがプレイヤーのインクの上か
	bool m_OnPlayerInk;
	// エネミーがジャンプ中か
	bool m_JumpNow;
	// 弾の発射レート
	int m_BulletRate=0;
	// エネミーの加速
	float m_Accel;
	// エネミーのジャンプ判定
	bool m_Jump;
	// エネミーとプレイヤーの距離
	float m_Distance;
	// エネミーがプレイヤーを発見しているか
	bool m_Found;
	// 射撃した回数
	int m_ShotCount;

	ID3D11VertexShader*		m_VertexShader = NULL;
	ID3D11PixelShader*		m_PixelShader = NULL;
	ID3D11InputLayout*		m_VertexLayout = NULL;

	BehaviorTree *m_AiTree;			// ビヘイビアツリー
	BehaviorData *m_BehaviorData;	// ビヘイビアデータ
	NodeBase *m_ActiveNode;			// 実行中ノード

public:
	Enemy() {};
	Enemy(BehaviorTree *ai_tree);

	static void Load();
	static void Unload();

	// キャラの体力
	const float m_MaxLife = 100.0f;
	float m_Life;
	// 弾の残量
	const float m_MaxInkTank = 100.0f;
	float m_InkTank;
	// キャラの視覚
	const float m_FoundDistance = 70.0f;

	void Init();
	void Uninit();
	void Update();
	void Draw();

	/* 基本動作 */
	void Shot();

	/* AIによる行動 */
	// ショット攻撃
	void ShotAttack();
	// ボム使用
	void UseBomb();
	// 潜伏（インクの回復と体力の回復）
	void Dive();
	// 自分の前方を塗る
	void ForwardPaint();
	// 追いかけるために塗る
	void ChasePaint();
	// 自分の下を塗る
	void UnderPaint();
	// 歩いてプレイヤー追跡
	void WalkChasePlayer();
	// 経路探索を用いて追跡
	void Pathfinder();
	// 未発見
	void Wait();

	void OnInk();
	void OnPlayerInk();

	// プレイヤーの状態取得
	bool GetEnemyState() { return m_EnemyState; }
	bool GetOnInk() { return m_OnInk; }
	bool GetOnPlayerInk() { return m_OnPlayerInk; }
	float GetLife() { return m_Life; }
	bool GetFound() { return m_Found; }

	// プレイヤーの弾の残量取得
	float GetInkTank() { return m_InkTank; }

	int GetShotCount() { return m_ShotCount; }
	void ShotReset() { m_ShotCount = 0; }

	// プレイヤーとの角度
	float Angle(float E1, float E2,float P1, float P2);
	float Angle(D3DXVECTOR3 enemy_Position, D3DXVECTOR3 player_Position);
	// プレイヤーとの距離
	float Distance(float Ex, float Ez, float Px, float Pz);

	// HPオーバー判定
	bool IsHpPercentOver(float check_percent);
	// インク量オーバー判定
	bool IsInkPercentOver(float check_percent);
	// プレイヤーとの距離判定
	bool IsDistanceOver(float check_distance);
};