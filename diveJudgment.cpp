#include "main.h"
#include "diveJudgment.h"
#include "enemy.h"

bool DiveJudgment::Judgment(Enemy *enemy)
{
	// ‘Ì—Í‚ª’á‚¢ê‡
	if (enemy->IsHpPercentOver(30.0f) == false)
	{
		int random = rand() % 100;

		if (random <= 70)
		{
			return true;
		}
	}

	// ƒCƒ“ƒNŽc—Ê‚ª’á‚¢ê‡
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
