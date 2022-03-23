#pragma once

#include "manager.h"

class OptionBar
{
private:
	ID3D11VertexShader*		m_VertexShader = NULL;
	ID3D11PixelShader*		m_PixelShader = NULL;
	ID3D11InputLayout*		m_VertexLayout = NULL;

	ID3D11Buffer*				m_VertexBuffer = NULL;
	ID3D11ShaderResourceView*	m_Texture = NULL;
	ID3D11ShaderResourceView*	m_Texture2 = NULL;

	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �o�[�̏�Ƀ}�E�X�J�[�\�������邩
	bool m_OnMouse = false;
	// �}�E�X�Ńz�[���h���Ă��邩
	bool m_HoldMouse = false;
	// �o�[�̐��l
	float m_BarPoint = 0.0f;
	// BGM�̐��l
	static float m_BarPointBGM;
	// SE�̐��l
	static float m_BarPointSE;
	// �o�[�̐��l���ς������
	static bool m_PointChange;
public:

	void Init(int Num);		// ������
	void Uninit();			// �I��
	void Update(int Num);	// �X�V
	void Draw();			// �`��

	static float GetBarPointBGM() { return m_BarPointBGM; }
	static float GetBarPointSE() { return m_BarPointSE; }
	static bool GetPointChange() { return m_PointChange; }
	static void SetPointChange(bool Set) { m_PointChange = Set; }
};
