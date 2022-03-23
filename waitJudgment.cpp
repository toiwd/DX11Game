#include "main.h"
#include "waitJudgment.h"
#include "enemy.h"

bool WaitJudgment::Judgment(Enemy *enemy)
{
	// エネミーがプレイヤーを未発見
	if (enemy->GetFound()==false)
	{
		return true;
	}

	return false;
}
