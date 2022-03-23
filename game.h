#pragma once

#include "scene.h"

class Game : public Scene
{
private:
	// Å‰‚¾‚¯“Ç‚İ‚Ş—p
	bool m_FirstLoad;

public:
	void Init();
	void Uninit();
	void Update();

};