#include "main.h"
#include "waitJudgment.h"
#include "enemy.h"

bool WaitJudgment::Judgment(Enemy *enemy)
{
	// �G�l�~�[���v���C���[�𖢔���
	if (enemy->GetFound()==false)
	{
		return true;
	}

	return false;
}
