#include "main.h"
#include "waitAction.h"
#include "enemy.h"

ActionBase::STATE WaitAction::Run(Enemy *enemy)
{
	enemy->Wait();

	return ActionBase::STATE::COMPLETE;
}