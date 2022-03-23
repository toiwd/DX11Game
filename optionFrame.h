#pragma once

#include "manager.h"

class OptionFrame
{
private:
	static ID3D11VertexShader*		m_VertexShader;
	static ID3D11PixelShader*		m_PixelShader;
	static ID3D11InputLayout*		m_VertexLayout;

	static ID3D11Buffer*				m_VertexBuffer;
	static ID3D11ShaderResourceView*	m_Texture;

	static int m_SelectFrame;
public:
	static void Init();		// ‰Šú‰»
	static void Uninit();	// I—¹
	static void Update();	// XV
	static void Draw();		// •`‰æ


};
