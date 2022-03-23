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

	// フェードイン・アウトの判定
	static int m_Fade;
	// 遷移するシーン番号
	static int m_NextScene;
	// 明るさを変える変数
	static float m_FadeLight;

public:
	static void Init();		// 初期化
	static void Uninit();	// 終了
	static void Update();	// 更新
	static void Draw();		// 描画

	static void SetFade(int Fade,int NextScene)
	{
		m_Fade = Fade;
		m_NextScene = NextScene;
	}
	
};