#pragma once

#include "manager.h"

class Option
{
private:
	// Optionが開いているかの判定
	static bool m_OptionMode;
	static bool m_OptionEnd;

public:
	static void Init();		// 初期化
	static void Uninit();	// 終了
	static void Update();	// 更新
	static void Draw();		// 描画

	static void SetOptionMode(bool OptionMode) { m_OptionMode = OptionMode;}
	static void SetOptionEnd(bool OptionEnd) { m_OptionEnd = OptionEnd;}

	static bool GetOptionMode() { return m_OptionMode; }
	static bool GetOptionEnd() { return m_OptionEnd; }

};