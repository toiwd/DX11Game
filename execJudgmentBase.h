#ifndef EXEC_JUDGMENT_BASE_H_
#define EXEC_JUDGMENT_BASE_H_

class Enemy;

// ���s����
class ExecJudgmentBase
{
public:
	virtual bool Judgment(Enemy *enemy) = 0;
};

#endif
