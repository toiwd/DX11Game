#include "main.h"
#include "shotJudgment.h"
#include "enemy.h"

bool ShotJudgment::Judgment(Enemy *enemy)
{
	if (enemy->IsInkPercentOver(71.0f))
	{
		int random = rand() % 50;

		if (random <= 1)
		{
			return false;
		}
	}

	return true;
}