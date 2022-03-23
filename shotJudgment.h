#ifndef BOMB_ATTACK_JUDGMENT_H_
#define BOMB_ATTACK_JUDGMENT_H_

#include "execJudgmentBase.h"

class Enemy;

// É{ÉÄçsìÆîªíË
class ShotJudgment : public ExecJudgmentBase
{
public:
	static ShotJudgment *GetInstance()
	{
		static ShotJudgment m_Instance;
		return &m_Instance;
	}

	virtual bool Judgment(Enemy *enemy);
};

#endif
