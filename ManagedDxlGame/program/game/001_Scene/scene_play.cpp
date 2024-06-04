#include "../../dxlib_ext/dxlib_ext.h"
#include "../000_Manager/game_manager.h"
#include "scene_play.h"
#include "scene_end.h"
#include "scene_base.h"
#include "../002_Camera/camera.h"
#include "../003_Object/object.h"
#include "../003_Object/Stage/stage.h"
#include "../003_Object/character/Player/player.h"
#include "../003_Object/character/Enemy/enemy.h"
#include "../003_Object/Bullet/bullet.h"
#include "../005_music/MusicManager.h"
#include "../006_Csvload/loadcsv.h"

//----------------------------------------------------------------------------------------
//	コンストラクタ
//----------------------------------------------------------------------------------------

ScenePlay::ScenePlay()
{

	Factory();	

}

//----------------------------------------------------------------------------------------
//	デストラクタ
//----------------------------------------------------------------------------------------

ScenePlay::~ScenePlay()
{
	object_list_.clear();
	bullet_list_.clear();
	enemy_list_.clear();
}

//----------------------------------------------------------------------------------------
//	アップデート
//----------------------------------------------------------------------------------------


void ScenePlay::Update(float delta_time)
{
	for (auto obj : object_list_) {
		obj->Update(delta_time);

	}

	Collision();
	camera_->target_ = player_->getPos();
	camera_->update();
	sequence_.update(delta_time);
	
}
//----------------------------------------------------------------------------------------
//	描画
//----------------------------------------------------------------------------------------

void ScenePlay::Draw()
{
	SetBackgroundColor(50, 50, 50);
	for (auto obj : object_list_) {
		obj->Draw(camera_);
	}
	DrawGridGround(camera_);
}

//----------------------------------------------------------------------------------------
//	プレイアイドル
//----------------------------------------------------------------------------------------

bool ScenePlay::seqIdle(float delta_time)
{
	if (sequence_.isStart()) {
		InitMusic();
	}
	
	if (!player_->getIsActive()) {
		sequence_.change(&ScenePlay::seqExplanation);
	}
	return true;
}

//----------------------------------------------------------------------------------------
//	リザルト　
//----------------------------------------------------------------------------------------

bool ScenePlay::seqExplanation(float delta_time)
{

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		auto mgr = GameManager::GetInstance();
		mgr->ChangeScene(new SceneEnd());
	}
	return true;
}

//----------------------------------------------------------------------------------------
//	ファクトリー　一括管理
//----------------------------------------------------------------------------------------

void ScenePlay::Factory()
{
	LoadCsv loader;
	enemy_data_ = loader.LoadEnemyDate("csv/enemy.csv");
	camera_ = std::make_shared<PlayerCamera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
	player_ = std::make_shared<Player>();
	player_->SetScenePlay(this);
	object_list_.emplace_back(player_);
	/*stage_ = std::make_shared<Stage>();
	object_list_.emplace_back(stage_);*/
	for (int i = 0; i < 20; i++) {
		bullet_list_.emplace_back(std::make_shared<Bullet>());
	}
	for (auto blt : bullet_list_) {
		object_list_.emplace_back(blt);
	}
	for (const auto& [id, date] : enemy_data_) {
		auto enemy = std::make_shared<Enemy>(player_);
		enemy->SetEnemyHP(date.enemy_hp);
		enemy->SetEnemyScore(date.enemy_score);
		enemy->SetCharacterSize(date.enemy_size);
		enemy->SetEnemySpeed(date.enemy_speed);
		enemy->SetEnemyDamage(date.enemy_damage);
		enemy->SetEnemyType(date.enemy_type);
		enemy->SetEnemyModel(date.enemy_model,date.enemy_texture);
		enemy->SetScenePlay(this);
		enemy->setIsActive(true);
		enemy_list_.emplace_back(enemy);
	}

	for (auto ene : enemy_list_) {
		object_list_.emplace_back(ene);
	}

}

//----------------------------------------------------------------------------------------
//	当たり判定　～	球とエネミー	～
//----------------------------------------------------------------------------------------

void ScenePlay::Collision()
{
	for (auto obj1 : enemy_list_) {
		for (auto obj2 : bullet_list_) {
			if(tnl::IsIntersectSphere(obj1->getPos(), obj1->getCollistionRadius(), obj2->getPos(), obj2->getCollistionRadius())){
//				if (obj1->GetType() == obj2->GetType()) { continue; }
				if (!obj2->getIsActive()) continue;
  				obj2->OnHit(obj1);
			}
		}
	}

}

//----------------------------------------------------------------------------------------
//	当たり判定　～	エネミーとプレイヤー　～
//----------------------------------------------------------------------------------------


void ScenePlay::CollisionPB()
{

}

//----------------------------------------------------------------------------------------
//	Musicの生成・再生
//----------------------------------------------------------------------------------------

void ScenePlay::InitMusic()
{	
	//	BGMのロードと再生
	MusicManager::GetInstance().LoadBGM("sound/playscene.wav");
	MusicManager::GetInstance().PlayBGM();

	//	SEのロードと再生
	MusicManager::GetInstance().LoadSE("shoot","sound/shoot.mp3");		//発射
	//MusicManager::GetInstance().LoadSE("shoot", "sound/shoot.mp3");	//撃破
}

//----------------------------------------------------------------------------------------
//	Bulletの生成
//----------------------------------------------------------------------------------------

void ScenePlay::SpawnBullet(tnl::Vector3 pos_, tnl::Quaternion rot_,Object::ObjectType type,int damage)
{
	auto blt = bullet_list_.begin();
	while ( blt != bullet_list_.end())
	{
		if (!(*blt)->getIsActive()) {			
			(*blt)->Spawn(pos_, rot_,type,damage);
			
			MusicManager::GetInstance().PlaySE("shoot");
			break;
		}

		blt++;
	}
	
}


