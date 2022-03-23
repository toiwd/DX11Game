#include "main.h"
#include "bombAction.h"
#include "enemy.h"

ActionBase::STATE BombAction::Run(Enemy *enemy)
{
	enemy->UseBomb();

	return ActionBase::STATE::COMPLETE;
}