#pragma once

#include "gameObject.h"


class Polygon2D : public GameObject
{
private:

	ID3D11VertexShader*		m_VertexShader = NULL;
	ID3D11PixelShader*		m_PixelShader = NULL;
	ID3D11InputLayout*		m_VertexLayout = NULL;

	static ID3D11Buffer*				m_VertexBuffer;
	static ID3D11ShaderResourceView*	m_Texture;

	int m_Num = 0;
	int m_Digit = 9;
	const int m_DigitNum = 6;
	const int m_MaxNum = 99999;
	
	D3DXVECTOR2 m_Size = D3DXVECTOR2(20.0f, 50.0f);

public:
	Polygon2D() {};
	Polygon2D(int num);

	static void Load();
	static void Unload();
	void Init();
	void Uninit();
	void Update();
	void Draw();

};