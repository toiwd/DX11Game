#ifndef WALK_CHASE_ACTION_H_
#define WALK_CHASE_ACTION_H_

#include "actionBase.h"

// ÉvÉåÉCÉÑÅ[í«ê’
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
