#include "main.h"
#include "walkChaseAction.h"
#include "enemy.h"

ActionBase::STATE WalkChaseAction::Run(Enemy *enemy)
{
	if (enemy->GetShotCount() <= 2 || enemy->GetOnPlayerInk() == true)
	{
		enemy->ForwardPaint();
	}
	else
	{
		enemy->WalkChasePlayer();

		if (enemy->GetOnPlayerInk() == true)
		{
			enemy->ShotReset();
		}
	}

	return ActionBase::STATE::COMPLETE;
}