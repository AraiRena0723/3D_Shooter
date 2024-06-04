#pragma once
#include "scene_base.h"
#include "../003_Object/object.h"
#include "../006_Csvload/loadcsv.h"

class PlayerCamera;
class Object;
class Character;
class Player;
class Enemy;
class Bullet;
class Stage;
class MusicManager;

class ScenePlay : public SceneBase  {
public:
	ScenePlay();
	~ScenePlay()override;
	void Update(float delta_time) override;
	void Draw()override;

	//-------------------------------------------------------------------------------------------
	void SpawnBullet(tnl::Vector3 pos_ ,tnl::Quaternion rot_,Object::ObjectType type,int damage );


private:

	//-------------------------------------------------------------------------------------------

	tnl::Sequence<ScenePlay> sequence_ = tnl::Sequence<ScenePlay>(this, &ScenePlay::seqIdle);
	Shared<Player> player_ = nullptr;
	Shared<PlayerCamera> camera_ = nullptr;
	Shared<Stage> stage_ = nullptr;
	std::list<std::shared_ptr<Enemy>> enemy_list_;
	std::list<std::shared_ptr<Bullet>> bullet_list_;
	std::unordered_map<int, EnemyDate> enemy_data_;
	bool seqIdle(float delta_time);
	bool seqExplanation(float delta_time);
	int serect_ = 0;
	
	//-------------------------------------------------------------------------------------------
	void Factory();
	void Collision();
	void CollisionPB();
	void Reset();
	void InitMusic();
	
};