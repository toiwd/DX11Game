#ifndef BOMB_ACTION_H_
#define BOMB_ACTION_H_

#include "actionBase.h"

// ƒ{ƒ€Ëo
class BombAction : public ActionBase
{
public:
	static BombAction *GetInstance()
	{
		static BombAction instance;
		return &instance;
	}

	virtual STATE Run(Enemy *enemy);
};

#endif
