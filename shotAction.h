
#include "ActionBase.h"

class Enemy;

// •¨—UŒ‚s“®
class ShotAction : public ActionBase
{
public:
	static ShotAction *GetInstance()
	{
		static ShotAction instance;
		return &instance;
	}

	virtual STATE Run(Enemy *enemy);
};

