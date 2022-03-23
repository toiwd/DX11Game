#ifndef WALK_JUDGMENT_H_
#define WALK_JUDGMENT_H_

#include "ExecJudgmentBase.h"

class Enemy;

// �ǐ՗p����p�N���X
class WalkJudgment : public ExecJudgmentBase
{
public:
	static WalkJudgment *GetInstance()
	{
		static WalkJudgment m_Instance;
		return &m_Instance;
	}

	// ����
	virtual bool Judgment(Enemy *enemy);
};

#endif
