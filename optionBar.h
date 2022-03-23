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

	// バーの上にマウスカーソルがあるか
	bool m_OnMouse = false;
	// マウスでホールドしているか
	bool m_HoldMouse = false;
	// バーの数値
	float m_BarPoint = 0.0f;
	// BGMの数値
	static float m_BarPointBGM;
	// SEの数値
	static float m_BarPointSE;
	// バーの数値が変わったか
	static bool m_PointChange;
public:

	void Init(int Num);		// 初期化
	void Uninit();			// 終了
	void Update(int Num);	// 更新
	void Draw();			// 描画

	static float GetBarPointBGM() { return m_BarPointBGM; }
	static float GetBarPointSE() { return m_BarPointSE; }
	static bool GetPointChange() { return m_PointChange; }
	static void SetPointChange(bool Set) { m_PointChange = Set; }
};
