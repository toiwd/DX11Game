#pragma once

#include "gameObject.h"

class Player : public GameObject
{

private:
	static class Model* m_ModelHuman;
	static class Model* m_ModelSquid;

	// �v���C���[�̏��
	bool m_PlayerState;
	// �v���C���[���C���N�̏ォ
	bool m_OnInk;
	// �v���C���[���G�l�~�[�̃C���N�̏ォ
	bool m_OnEnemyInk;
	// �v���C���[���W�����v����
	bool m_JumpNow;
	// �e�̔��˃��[�g
	int m_BulletRate;
	// �v���C���[�̉���
	float m_Accel;
	// �v���C���[�̃}�E�X���x
	static float m_Sensi;
	// �v���C���[�̐�������
	bool m_Dead;
	// ���S��̃N�[���^�C��
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

	// �L�����̗̑�
	const float m_MaxLife = 100.0f;
	float m_Life;
	// �e�̎c��
	const float m_MaxInkTank = 100.0f;
	float m_InkTank;

	// �v���C���[�̏�Ԏ擾
	bool GetPlayerState() { return m_PlayerState; }
	bool GetOnInk() { return m_OnInk; }
	bool GetOnEnemyInk() { return m_OnEnemyInk; }
	bool GetDead() { return m_Dead; }
	float GetLife() { return m_Life; }

	// �v���C���[�̒e�̎c�ʎ擾
	float GetInkTank() { return m_InkTank; }

	// �v���C���[�̃}�E�X���x�ݒ�
	static void SetPlayerSens(float Sens)
	{
		if (m_Sensi == Sens) return;
		m_Sensi = Sens;
	}
	
};