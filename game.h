#pragma once

#include "scene.h"

class Game : public Scene
{
private:
	// �ŏ������ǂݍ��ޗp
	bool m_FirstLoad;

public:
	void Init();
	void Uninit();
	void Update();

};