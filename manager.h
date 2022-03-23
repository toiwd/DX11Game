#pragma once

#include "scene.h"

class Manager // クラス
{
private:
	static class Scene* m_Scene;

public:
	// メンバ関数またはメソッド
	static void Init();		// 初期化
	static void Uninit();	// 終了
	static void Update();	// 更新
	static void Draw();		// 描画

	static class Scene* GetScene() { return m_Scene; }

	template <typename T>
	static void SetScene()
	{
		if (m_Scene)
		{
			// Titleを消してからGameを作る
			m_Scene->Uninit();
			delete m_Scene;
		}

		m_Scene = new T();
		m_Scene->Init();
	}
};