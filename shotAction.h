
#include "ActionBase.h"

class Enemy;

// �����U���s��
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

