#include "main.h"
#include "chaseJudgment.h"
#include "enemy.h"

bool ChaseJudgment::Judgment(Enemy *enemy)
{
	if (enemy->IsDistanceOver(8.0f))
	{
		return true;
	}

	return false;
}