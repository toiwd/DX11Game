#ifndef PATHFINDER_ACTION_H_
#define PATHFINDER_ACTION_H_

#include "actionBase.h"

// ÉvÉåÉCÉÑÅ[í«ê’
class PathfinderAction : public ActionBase
{
public:
	static PathfinderAction *GetInstance()
	{
		static PathfinderAction instance;
		return &instance;
	}

	virtual STATE Run(Enemy *enemy);
};

#endif
