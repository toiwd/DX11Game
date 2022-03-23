#include "main.h"
#include "manager.h"
#include "renderer.h"

#include "input.h"
#include "titleCamera.h"
#include "scene.h"
#include "mouse.h"

#include "result.h"
#include "titleLogo.h"
#include "resultLogo3D.h"
#include "meshField.h"
#include "ink.h"
#include "enemyInk.h"
#include "sky.h"
#include "result.h"
#include "field.h"
#include "polygon2D.h"

#include "title.h"
#include "fade.h"

void Result::Init()
{
	Ink::Load();
	EnemyInk::Load();

	AddGameObject<TitleCamera>(0);
	AddGameObject<MeshField>(1);
	AddGameObject<Sky>(1);
	AddGameObject<ResultLogo3D>(1);

	Scene* scene = Manager::GetScene();
	// 色を塗る
	for (int i = 0; i < 100; i++)
	{
		D3DXVECTOR3 pos;
		pos.x = (float)rand() / RAND_MAX * 100.0f - 50.0f;
		pos.z = (float)rand() / RAND_MAX * 100.0f - 50.0f;
		pos.y = 0.2f;
		scene->AddGameObject<Ink>(1)->SetPosition(pos);
	}

	for (int i = 0; i < 100; i++)
	{
		D3DXVECTOR3 pos;
		pos.x = (float)rand() / RAND_MAX * 100.0f - 50.0f;
		pos.z = (float)rand() / RAND_MAX * 100.0f - 50.0f;
		pos.y = 0.2f;
		scene->AddGameObject<EnemyInk>(1)->SetPosition(pos);
	}

	// 絶対座標モードに変更
	Mouse_SetMode(MOUSE_POSITION_MODE_ABSOLUTE);
}

void Result::Uninit()
{
	Scene::Uninit();// 継承元クラスのメソッド呼び出し
}

void Result::Update()
{
	Scene::Update();//継承元クラスのメソッドの呼び出し
	if (Input::GetKeyTrigger(VK_SPACE))
	{
		// 遷移先
		Fade::SetFade(1,1);
	}
}