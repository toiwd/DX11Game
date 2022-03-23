#pragma once

#include "gameObject.h"

class Block : public GameObject
{
private:
	class Model* m_Model;

	ID3D11VertexShader*		m_VertexShader = NULL;
	ID3D11PixelShader*		m_PixelShader = NULL;
	ID3D11InputLayout*		m_VertexLayout = NULL;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};
