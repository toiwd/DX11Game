#ifndef DIVE_JUDGMENT_H_
#define DIVE_JUDGMENT_H_

#include "execJudgmentBase.h"

// ������s����
class DiveJudgment : public ExecJudgmentBase
{
public:
	static DiveJudgment *GetInstance()
	{
		static DiveJudgment instance;
		return &instance;
	}

	virtual bool Judgment(Enemy *enemy);
};

#endif
