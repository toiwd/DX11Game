#pragma once

#include "manager.h"

class OptionUI
{
private:
	static ID3D11VertexShader*		m_VertexShader;
	static ID3D11PixelShader*		m_PixelShader;
	static ID3D11InputLayout*		m_VertexLayout;

	static ID3D11Buffer*				m_VertexBuffer;
	static ID3D11ShaderResourceView*	m_Texture;

public:
	static void Init();		// 初期化
	static void Uninit();	// 終了
	static void Update();	// 更新
	static void Draw();		// 描画


};
