#ifndef WAIT_ACTION_H_
#define WAIT_ACTION_H_

#include "actionBase.h"

class Enemy;

// 潜伏行動（インクの回復、体力の回復）
class WaitAction : public ActionBase
{
public:
	static WaitAction *GetInstance()
	{
		static WaitAction instance;
		return &instance;
	}

	virtual STATE Run(Enemy *enemy);
};

#endif
