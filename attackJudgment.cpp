#include "main.h"
#include <stdlib.h>
#include "attackJudgment.h"
#include "enemy.h"

bool AttackJudgment::Judgment(Enemy *enemy)
{
	int random = 0;

	if (enemy->IsHpPercentOver(50.0f))
	{
		return true;
	}
	else if (enemy->IsHpPercentOver(30.0f))
	{
		random = 80;
	}
	else
	{
		random = 50;
	}

	if (rand() % 100 <= random)
	{
		return true;
	}

	return false;
}