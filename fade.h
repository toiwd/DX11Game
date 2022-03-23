#pragma once

#include "manager.h"

class Fade
{
private:
	static ID3D11VertexShader*		m_VertexShader;
	static ID3D11PixelShader*		m_PixelShader;
	static ID3D11InputLayout*		m_VertexLayout;

	static ID3D11Buffer*				m_VertexBuffer;
	static ID3D11ShaderResourceView*	m_Texture;

	// �t�F�[�h�C���E�A�E�g�̔���
	static int m_Fade;
	// �J�ڂ���V�[���ԍ�
	static int m_NextScene;
	// ���邳��ς���ϐ�
	static float m_FadeLight;

public:
	static void Init();		// ������
	static void Uninit();	// �I��
	static void Update();	// �X�V
	static void Draw();		// �`��

	static void SetFade(int Fade,int NextScene)
	{
		m_Fade = Fade;
		m_NextScene = NextScene;
	}
	
};