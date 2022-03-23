#pragma once

#include "gameObject.h"

class EnemySecondBullet : public GameObject
{

private:
	static class Model* m_Model;

	ID3D11VertexShader*		m_VertexShader = NULL;
	ID3D11PixelShader*		m_PixelShader = NULL;
	ID3D11InputLayout*		m_VertexLayout = NULL;

	// ’e‚ÌŽË’ö
	float m_range;
	int m_BulletTime;

public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();
};
