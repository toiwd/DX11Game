#pragma once

#include "gameObject.h"

class TitleLogo3D : public GameObject
{

private:
	class Model* m_Model;

	ID3D11Buffer*				m_VertexBuffer = NULL;
	ID3D11ShaderResourceView*	m_Texture = NULL;

	ID3D11VertexShader*			m_VertexShader = NULL;
	ID3D11PixelShader*			m_PixelShader = NULL;
	ID3D11InputLayout*			m_VertexLayout = NULL;

	D3DXVECTOR2 m_Velocity = { 0.0f, 0.0f };
	float m_Gravity = 9.8f;
	float m_Mass = 1.0f;
	// マウスをクリックした位置
	D3DXVECTOR2 m_MouseDownPosition = { 0.0f,0.0f };
	// クリックした位置と現在地の差
	D3DXVECTOR2 m_MouseDiffernce = { 0.0f,0.0f };

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};
