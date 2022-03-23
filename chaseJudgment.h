#ifndef CHASE_JUDGMENT_H_
#define CHASE_JUDGMENT_H_

#include "ExecJudgmentBase.h"

class Enemy;

// �ǐ՗p����p�N���X
class ChaseJudgment : public ExecJudgmentBase
{
public:
	static ChaseJudgment *GetInstance()
	{
		static ChaseJudgment m_Instance;
		return &m_Instance;
	}

	// ����
	virtual bool Judgment(Enemy *enemy);
};

#endif
