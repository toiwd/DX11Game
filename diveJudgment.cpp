#include "main.h"
#include "diveJudgment.h"
#include "enemy.h"

bool DiveJudgment::Judgment(Enemy *enemy)
{
	// 体力が低い場合
	if (enemy->IsHpPercentOver(30.0f) == false)
	{
		int random = rand() % 100;

		if (random <= 70)
		{
			return true;
		}
	}

	// インク残量が低い場合
	if (enemy->IsInkPercentOver(30.0f) == false)
	{
		int random = rand() % 100;

		if (random <= 25)
		{
			return true;
		}
	}

	return false;
}
