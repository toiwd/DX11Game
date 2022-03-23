#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "mouse.h"
#include "option.h"
#include "optionUI.h"
#include "optionFrame.h"
#include "optionBar.h"
#include "player.h"

bool		Option::m_OptionMode = false;
bool		Option::m_OptionEnd = false;

OptionBar* bar1 = new OptionBar;
OptionBar* bar2 = new OptionBar;
OptionBar* bar3 = new OptionBar;

void Option::Init()
{
	OptionUI::Init();
	OptionFrame::Init();
	bar1->Init(0);
	bar2->Init(1);
	bar3->Init(2);
}


void Option::Uninit()
{
	bar1->Uninit();
	bar2->Uninit();
	bar3->Uninit();
	delete bar1;
	delete bar2;
	delete bar3;
	OptionFrame::Uninit();
	OptionUI::Uninit();
}

void Option::Update()
{
	if (m_OptionMode == false && Input::GetKeyTrigger(VK_TAB))
	{
		m_OptionMode = true;
		// 相対座標モードに変更
		Mouse_SetMode(MOUSE_POSITION_MODE_ABSOLUTE);
		
	}
	else if (m_OptionMode == true && Input::GetKeyTrigger(VK_TAB))
	{
		m_OptionMode = false;
		m_OptionEnd = true;
	}
	
	if (m_OptionMode == true)
	{
		OptionUI::Update();
		OptionFrame::Update();
		bar1->Update(0);
		bar2->Update(1);
		bar3->Update(2);
	}
}

void Option::Draw()
{
	// オプション状態でなければ描画しない
	if (m_OptionMode == false) return;

	OptionUI::Draw();
	OptionFrame::Draw();
	bar1->Draw();
	bar2->Draw();
	bar3->Draw();
}
