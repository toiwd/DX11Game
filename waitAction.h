#ifndef WAIT_ACTION_H_
#define WAIT_ACTION_H_

#include "actionBase.h"

class Enemy;

// �����s���i�C���N�̉񕜁A�̗͂̉񕜁j
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
