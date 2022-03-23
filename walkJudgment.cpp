#include "main.h"
#include "walkJudgment.h"
#include "enemy.h"

bool WalkJudgment::Judgment(Enemy *enemy)
{
	if (enemy->IsDistanceOver(5.0f))
	{
		return false;
	}

	return true;
}