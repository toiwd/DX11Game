#ifndef WAIT_JUDGMENT_H_
#define WAIT_JUDGMENT_H_

#include "execJudgmentBase.h"

// ������s����
class WaitJudgment : public ExecJudgmentBase
{
public:
	static WaitJudgment *GetInstance()
	{
		static WaitJudgment instance;
		return &instance;
	}

	virtual bool Judgment(Enemy *enemy);
};

#endif
