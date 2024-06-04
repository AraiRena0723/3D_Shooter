#pragma once
#include "../Character.h"
#include "../../../006_Csvload/loadcsv.h"

class Player;
class Character;
class Camera;

class Enemy : public Character {
public:
	Enemy(std::weak_ptr<Player>player);
	~Enemy();
	void Update(float delta_time) override;
	void Draw(Shared<dxe::Camera> camera) override;
	void OnHit(Shared<Object> object) override;
	enum class EnemyType {
		Normal,
		Speed,
		Power,
		Chase
	};
	//------------------------	Getter&Setter	----------------------------------//

	tnl::Vector3 GetEnemyPos() { return pos_; }
	tnl::Quaternion GetEnemyRot() { return rot_; }
	bool GetEnemyShoot() { return enemy_bullet_flag; }
	EnemyType GetEnemyType() { return enemytype; }

	void SetEnemyShoot(bool shoot_flag) { enemy_bullet_flag = shoot_flag; }
	void SetEnemyType(int type);
	void SetEnemyID(int id) { enemy_id_ = id; }
	void SetEnemyHP(int hp_) { character_hp_ = hp_; enemy_reset_hp = hp_; }
	void SetEnemyScore(int score_) { enemy_score = score_; }
	void SetEnemySpeed(float speed) { speed_ = speed; }
	void SetEnemyModel(const std::string& model, const std::string& texture);
	void SetEnemyDamage(int damage) { object_damage_ = damage; character_damage_ = damage; }
	void ReSetEnemy();


	
private:
	//------------------------	ポインタ	----------------------------------//
	std::weak_ptr<Player> player_ ;
	EnemyDate e_date;

	//------------------------	変数	----------------------------------//
	
	EnemyType enemytype = EnemyType::Normal;
	tnl::Sequence<Enemy> sequence_m_ = tnl::Sequence<Enemy>(this, &Enemy::SeqIdle);
	float rangedegree_;							//範囲（角度）
	float length_;								//長さ
	float bullet_interbal;						//方向（角度）
	float speed_;								//速度
	tnl::Quaternion rot_m_;						//
	int bullet_shoot;							//発射数							//
	bool enemy_bullet_flag;						//発射フラグ
	int enemy_score;							//キルポイント
	int enemy_id_;
	int enemy_reset_hp;
	std::string enem_model_;
	tnl::Vector3 move_pos;
	float range = 45.0f;						//視界の角度を決める

	//------------------------	関数	----------------------------------//
	bool SeqIdle(const float delta_time);
	bool Normal(const float delta_time);
	bool Speed(const float delta_time);
	bool Power(const float delta_time);
	bool Chase(const float delta_time);
	bool ChaseAttack(const float delta_time);
	bool MoveToPosition(const float delta_time);
	bool PlayerLookTrigger(tnl::Vector3 pos);	//改良が変
	tnl::Vector3 Vector(tnl::Vector3 pos);		//ベクトルを求める
	float InnerProduct(tnl::Vector3 vector1, tnl::Vector3 vector2);		//内積を求める式
	float Distance(const tnl::Vector3& pos);	//距離を求める

	tnl::Vector3 RandomPosition(const tnl::Vector3& center, float range);
};