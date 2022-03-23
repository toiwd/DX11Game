#ifndef ATTACK_JUDGMENT_H_
#define ATTACK_JUDGMENT_H_

#include "ExecJudgmentBase.h"

class Enemy;

// �U���p����p�N���X
class AttackJudgment : public ExecJudgmentBase
{
public:
	static AttackJudgment *GetInstance()
	{
		static AttackJudgment m_Instance;
		return &m_Instance;
	}

	// ����
	virtual bool Judgment(Enemy *enemy);
};

#endif
