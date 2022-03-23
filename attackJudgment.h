#ifndef ATTACK_JUDGMENT_H_
#define ATTACK_JUDGMENT_H_

#include "ExecJudgmentBase.h"

class Enemy;

// 攻撃用判定用クラス
class AttackJudgment : public ExecJudgmentBase
{
public:
	static AttackJudgment *GetInstance()
	{
		static AttackJudgment m_Instance;
		return &m_Instance;
	}

	// 判定
	virtual bool Judgment(Enemy *enemy);
};

#endif
