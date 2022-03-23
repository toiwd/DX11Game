#pragma once

#include "scene.h"

class Manager // �N���X
{
private:
	static class Scene* m_Scene;

public:
	// �����o�֐��܂��̓��\�b�h
	static void Init();		// ������
	static void Uninit();	// �I��
	static void Update();	// �X�V
	static void Draw();		// �`��

	static class Scene* GetScene() { return m_Scene; }

	template <typename T>
	static void SetScene()
	{
		if (m_Scene)
		{
			// Title�������Ă���Game�����
			m_Scene->Uninit();
			delete m_Scene;
		}

		m_Scene = new T();
		m_Scene->Init();
	}
};