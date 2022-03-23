#ifndef DIVE_ACTION_H_
#define DIVE_ACTION_H_

#include "actionBase.h"

class Enemy;

// �����s���i�C���N�̉񕜁A�̗͂̉񕜁j
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
