#include "main.h"
#include "diveAction.h"
#include "enemy.h"

ActionBase::STATE DiveAction::Run(Enemy *enemy)
{
	// ƒCƒ“ƒN‚Ìã‚È‚ç
	if (enemy->GetOnInk() == true)
	{
		// ö‚Á‚Ä‰ñ•œ
		enemy->Dive();
	}
	else
	{
		// ‚»‚¤‚Å‚È‚¢‚È‚çŽ©•ª‚Ì‰º‚ð“h‚é
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