#ifndef CHASE_JUDGMENT_H_
#define CHASE_JUDGMENT_H_

#include "ExecJudgmentBase.h"

class Enemy;

// 追跡用判定用クラス
class ChaseJudgment : public ExecJudgmentBase
{
public:
	static ChaseJudgment *GetInstance()
	{
		static ChaseJudgment m_Instance;
		return &m_Instance;
	}

	// 判定
	virtual bool Judgment(Enemy *enemy);
};

#endif
