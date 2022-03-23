#include "main.h"
#include "diveJudgment.h"
#include "enemy.h"

bool DiveJudgment::Judgment(Enemy *enemy)
{
	// �̗͂��Ⴂ�ꍇ
	if (enemy->IsHpPercentOver(30.0f) == false)
	{
		int random = rand() % 100;

		if (random <= 70)
		{
			return true;
		}
	}

	// �C���N�c�ʂ��Ⴂ�ꍇ
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
