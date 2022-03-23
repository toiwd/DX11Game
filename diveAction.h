#ifndef DIVE_ACTION_H_
#define DIVE_ACTION_H_

#include "actionBase.h"

class Enemy;

// 潜伏行動（インクの回復、体力の回復）
class DiveAction : public ActionBase
{
public:
	static DiveAction *GetInstance()
	{
		static DiveAction instance;
		return &instance;
	}

	virtual STATE Run(Enemy *enemy);
};

#endif
