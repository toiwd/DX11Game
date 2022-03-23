#ifndef WALK_CHASE_ACTION_H_
#define WALK_CHASE_ACTION_H_

#include "actionBase.h"

// �v���C���[�ǐ�
class WalkChaseAction : public ActionBase
{
public:
	static WalkChaseAction *GetInstance()
	{
		static WalkChaseAction instance;
		return &instance;
	}

	virtual STATE Run(Enemy *enemy);
};

#endif
