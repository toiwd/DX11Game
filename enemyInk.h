#pragma once

#include "gameObject.h"

class EnemyInk : public GameObject
{
private:
	ID3D11VertexShader*		m_VertexShader = NULL;
	ID3D11PixelShader*		m_PixelShader = NULL;
	ID3D11InputLayout*		m_VertexLayout = NULL;

	static ID3D11Buffer*				m_VertexBuffer;
	static ID3D11ShaderResourceView*	m_Texture;
	static ID3D11ShaderResourceView*	m_NormalTexture;

	D3DXVECTOR4 m_Time;
	int m_Count = 0;

public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static void Paint(D3DXVECTOR3 position);
};