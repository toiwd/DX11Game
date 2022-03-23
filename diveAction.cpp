#include "main.h"
#include "diveAction.h"
#include "enemy.h"

ActionBase::STATE DiveAction::Run(Enemy *enemy)
{
	// �C���N�̏�Ȃ�
	if (enemy->GetOnInk() == true)
	{
		// �����ĉ�
		enemy->Dive();
	}
	else
	{
		// �����łȂ��Ȃ玩���̉���h��
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