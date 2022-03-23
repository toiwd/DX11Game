#ifndef WALK_JUDGMENT_H_
#define WALK_JUDGMENT_H_

#include "ExecJudgmentBase.h"

class Enemy;

// 追跡用判定用クラス
class WalkJudgment : public ExecJudgmentBase
{
public:
	static WalkJudgment *GetInstance()
	{
		static WalkJudgment m_Instance;
		return &m_Instance;
	}

	// 判定
	virtual bool Judgment(Enemy *enemy);
};

#endif
