#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"
#include "audio.h"
#include "game.h"
#include "title.h"
#include "result.h"
#include "fade.h"
#include "option.h"


Scene* Manager::m_Scene = NULL;

// �֐��̑O�ɃN���X��::
void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	Audio::InitMaster();
	Fade::Init();
	Option::Init();
	
	SetScene<Title>();
}


void Manager::Uninit()
{
	m_Scene->Uninit();
	delete m_Scene;

	Audio::UninitMaster();
	Option::Uninit();
	Fade::Uninit();
	Input::Uninit();
	Renderer::Uninit();
}

void Manager::Update()
{
	Input::Update(); // �擪�ŌĂяo��
	Fade::Update();
	Option::Update();

	m_Scene->Update();
}

void Manager::Draw()
{
	Renderer::Begin();

	m_Scene->Draw();
	Option::Draw();
	Fade::Draw();
	
	Renderer::End();
}
