#include "main.h"
#include "manager.h"
#include "renderer.h"
#include <time.h>

#include "input.h"
#include "camera.h"
#include "field.h"
#include "meshField.h"
#include "model.h"
#include "player.h"
#include "enemy.h"
#include "enemyHpBar.h"
#include "HpBar.h"
#include "bullet.h"
#include "secondBullet.h"
#include "enemyBullet.h"
#include "enemySecondBullet.h"
#include "polygon2D.h"
#include "sky.h"
#include "wall.h"
#include "block.h"
#include "explosion.h"
#include "ink.h"
#include "enemyInk.h"
#include "crosshair.h"
#include "tank.h"
#include "inkinTank.h"
#include "bomb.h"
#include "bombSplash.h"
#include "enemyBomb.h"
#include "enemyBombSplash.h"
#include "audio.h"
#include "mouse.h"
#include "option.h"
#include "optionBar.h"
#include "inputBehavior.h"

#include "aStar.h"

#include "game.h"
#include "result.h"
#include "fade.h"

void Game::Init()
{
	srand((unsigned)time(NULL));

	Player::Load();
	Bullet::Load();
	SecondBullet::Load();
	Enemy::Load();
	EnemyBullet::Load();
	EnemySecondBullet::Load();
	Explosion::Load();
	Ink::Load();
	EnemyInk::Load();
	EnemyHpBar::Load();
	HpBar::Load();
	Tank::Load();
	InkInTank::Load();
	Bomb::Load();
	BombSplash::Load();
	EnemyBomb::Load();
	EnemyBombSplash::Load();

	InputBehavior::InputAi();


	AddGameObject<Camera>(0);
	AddGameObject<MeshField>(1);
	AddGameObject<Sky>(1);
	AddGameObject<Player>(1);
	AddGameObject<Tank>(1);
	AddGameObject<InkInTank>(1);

	AddGameObject<Crosshair>(2);
	AddGameObject<HpBar>(2);

	CreateMap();
	
	Audio *bgm = AddGameObject<Audio>(0); // レイヤ番号はどこでもよい
	bgm->Load("asset\\audio\\BGM1.wav");
	bgm->Play(true);

	m_FirstLoad = true;
}

void Game::Uninit()
{
	Scene::Uninit();// 継承元クラスのメソッド呼び出し

	EnemyBombSplash::Unload();
	EnemyBomb::Unload();
	BombSplash::Unload();
	Bomb::Unload();
	InkInTank::Unload();
	Tank::Unload();
	EnemyInk::Unload();
	Ink::Unload();
	Explosion::Unload();
	EnemySecondBullet::Unload();
	EnemyBullet::Unload();
	EnemyHpBar::Unload();
	HpBar::Unload();
	SecondBullet::Unload();
	Bullet::Unload();
	Enemy::Unload();
	Player::Unload();
}

void Game::Update()// ゲームを管理する条件を入れる
{
	// Option内の数値が変わっていたら または
	// 最初に読み込まれた時に 実行
	if (OptionBar::GetPointChange() == true || m_FirstLoad == true)
	{
		// BGMの音量変更
		Audio* bgm = Manager::GetScene()->GetGameObject<Audio>(0);
		bgm->SetVolume(OptionBar::GetBarPointBGM() / 600.0f);

		// SEの音量変更
		Audio* se = Manager::GetScene()->GetGameObject<Audio>(1);
		se->SetVolume(OptionBar::GetBarPointSE() / 600.0f);

		OptionBar::SetPointChange(false);
		m_FirstLoad = false;
	}

	if (Option::GetOptionMode() == false)
	{
		Scene::Update();//継承元クラスのメソッドの呼び出し
	}

	std::vector<Enemy*>enemyList = GetGameObjects<Enemy>(1);

	if (enemyList.size() == 0)
	{
		Fade::SetFade(1,3);
	}
}