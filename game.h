#pragma once

#include "scene.h"

class Game : public Scene
{
private:
	// 最初だけ読み込む用
	bool m_FirstLoad;

public:
	void Init();
	void Uninit();
	void Update();

};