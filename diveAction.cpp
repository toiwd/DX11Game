#include "main.h"
#include "diveAction.h"
#include "enemy.h"

ActionBase::STATE DiveAction::Run(Enemy *enemy)
{
	// インクの上なら
	if (enemy->GetOnInk() == true)
	{
		// 潜って回復
		enemy->Dive();
	}
	else
	{
		// そうでないなら自分の下を塗る
		enemy->UnderPaint();
	}
	
	if (enemy->IsHpPercentOver(100.0f) && enemy->IsInkPercentOver(100.0f))
	{
		return ActionBase::STATE::COMPLETE;
	}
	else
	{
		return ActionBase::STATE::RUN;
	}
	
}