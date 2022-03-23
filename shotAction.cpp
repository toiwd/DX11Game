#include "main.h"
#include <stdio.h>
#include "shotAction.h"
#include "enemy.h"

ActionBase::STATE ShotAction::Run(Enemy *enemy)
{
	enemy->ShotAttack();

	return ActionBase::STATE::COMPLETE;
}
