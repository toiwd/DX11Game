#ifndef PATHFINDER_ACTION_H_
#define PATHFINDER_ACTION_H_

#include "actionBase.h"

// �v���C���[�ǐ�
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
