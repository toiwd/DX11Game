#pragma once

#include "manager.h"

class Option
{
private:
	// Option���J���Ă��邩�̔���
	static bool m_OptionMode;
	static bool m_OptionEnd;

public:
	static void Init();		// ������
	static void Uninit();	// �I��
	static void Update();	// �X�V
	static void Draw();		// �`��

	static void SetOptionMode(bool OptionMode) { m_OptionMode = OptionMode;}
	static void SetOptionEnd(bool OptionEnd) { m_OptionEnd = OptionEnd;}

	static bool GetOptionMode() { return m_OptionMode; }
	static bool GetOptionEnd() { return m_OptionEnd; }

};