#include "main.h"
#include "pathfinderAction.h"
#include "enemy.h"

ActionBase::STATE PathfinderAction::Run(Enemy *enemy)
{
	if (enemy->GetShotCount() <= 2 || enemy->GetOnPlayerInk() == true)
	{
		enemy->ChasePaint();
	}
	else
	{
		enemy->Pathfinder();

		if (enemy->GetOnPlayerInk() == true)
		{
			enemy->ShotReset();
		}
	}

	return ActionBase::STATE::COMPLETE;
}