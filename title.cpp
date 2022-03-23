#include "main.h"
#include "manager.h"
#include "renderer.h"

#include "input.h"
#include "titleCamera.h"
#include "scene.h"
#include "mouse.h"

#include "title.h"
#include "titleLogo.h"
#include "titleLogo3D.h"
#include "meshField.h"
#include "ink.h"
#include "enemyInk.h"
#include "sky.h"
#include "result.h"
#include "field.h"
#include "polygon2D.h"
#include "option.h"

#include "game.h"
#include "fade.h"

void Title::Init()
{
	Ink::Load();
	EnemyInk::Load();

	AddGameObject<TitleCamera>(0);
	AddGameObject<MeshField>(1);
	AddGameObject<Sky>(1);
	AddGameObject<TitleLogo3D>(1);

	Scene* scene = Manager::GetScene();
	// �F��h��
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
	
	// ��΍��W���[�h�ɕύX
	Mouse_SetMode(MOUSE_POSITION_MODE_ABSOLUTE);
}

void Title::Uninit()
{
	EnemyInk::Unload();
	Ink::Unload();
	Scene::Uninit();// �p�����N���X�̃��\�b�h�Ăяo��
}

void Title::Update()
{
	if (Option::GetOptionMode() == false)
	{
		Scene::Update();//�p�����N���X�̃��\�b�h�̌Ăяo��
	}

	if (Input::GetKeyTrigger(VK_SPACE))
	{
		// �J�ڐ�
		Fade::SetFade(1,2);
	}
}
