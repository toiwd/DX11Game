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

	// �G�l�~�[�̏��
	bool m_EnemyState=false;
	// �G�l�~�[���C���N�̏ォ
	bool m_OnInk;
	// �G�l�~�[���v���C���[�̃C���N�̏ォ
	bool m_OnPlayerInk;
	// �G�l�~�[���W�����v����
	bool m_JumpNow;
	// �e�̔��˃��[�g
	int m_BulletRate=0;
	// �G�l�~�[�̉���
	float m_Accel;
	// �G�l�~�[�̃W�����v����
	bool m_Jump;
	// �G�l�~�[�ƃv���C���[�̋���
	float m_Distance;
	// �G�l�~�[���v���C���[�𔭌����Ă��邩
	bool m_Found;
	// �ˌ�������
	int m_ShotCount;

	ID3D11VertexShader*		m_VertexShader = NULL;
	ID3D11PixelShader*		m_PixelShader = NULL;
	ID3D11InputLayout*		m_VertexLayout = NULL;

	BehaviorTree *m_AiTree;			// �r�w�C�r�A�c���[
	BehaviorData *m_BehaviorData;	// �r�w�C�r�A�f�[�^
	NodeBase *m_ActiveNode;			// ���s���m�[�h

public:
	Enemy() {};
	Enemy(BehaviorTree *ai_tree);

	static void Load();
	static void Unload();

	// �L�����̗̑�
	const float m_MaxLife = 100.0f;
	float m_Life;
	// �e�̎c��
	const float m_MaxInkTank = 100.0f;
	float m_InkTank;
	// �L�����̎��o
	const float m_FoundDistance = 70.0f;

	void Init();
	void Uninit();
	void Update();
	void Draw();

	/* ��{���� */
	void Shot();

	/* AI�ɂ��s�� */
	// �V���b�g�U��
	void ShotAttack();
	// �{���g�p
	void UseBomb();
	// �����i�C���N�̉񕜂Ƒ̗͂̉񕜁j
	void Dive();
	// �����̑O����h��
	void ForwardPaint();
	// �ǂ������邽�߂ɓh��
	void ChasePaint();
	// �����̉���h��
	void UnderPaint();
	// �����ăv���C���[�ǐ�
	void WalkChasePlayer();
	// �o�H�T����p���Ēǐ�
	void Pathfinder();
	// ������
	void Wait();

	void OnInk();
	void OnPlayerInk();

	// �v���C���[�̏�Ԏ擾
	bool GetEnemyState() { return m_EnemyState; }
	bool GetOnInk() { return m_OnInk; }
	bool GetOnPlayerInk() { return m_OnPlayerInk; }
	float GetLife() { return m_Life; }
	bool GetFound() { return m_Found; }

	// �v���C���[�̒e�̎c�ʎ擾
	float GetInkTank() { return m_InkTank; }

	int GetShotCount() { return m_ShotCount; }
	void ShotReset() { m_ShotCount = 0; }

	// �v���C���[�Ƃ̊p�x
	float Angle(float E1, float E2,float P1, float P2);
	float Angle(D3DXVECTOR3 enemy_Position, D3DXVECTOR3 player_Position);
	// �v���C���[�Ƃ̋���
	float Distance(float Ex, float Ez, float Px, float Pz);

	// HP�I�[�o�[����
	bool IsHpPercentOver(float check_percent);
	// �C���N�ʃI�[�o�[����
	bool IsInkPercentOver(float check_percent);
	// �v���C���[�Ƃ̋�������
	bool IsDistanceOver(float check_distance);
};