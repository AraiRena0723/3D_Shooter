#pragma once
#include "../Character.h"

class Player;
class Character;
class Enemy : public Character {
public:
	Enemy();
	~Enemy();
	void Update(float delta_time) override;
	void Draw(Shared<dxe::Camera> camera) override;
	enum class EnemyType {
		Normal,
		Speed,
		Power,
		Chase
	};
	//------------------------	Getter&Setter	----------------------------------//

	tnl::Vector3 GetEnemyPos() { return pos_; }
	void SetEnemyHp(int damage) { character_hp_ -= damage; }
	
private:
	//------------------------	ポインタ	----------------------------------//
	Shared<Player> player = nullptr;

	//------------------------	変数	----------------------------------//
	EnemyType enemytype = EnemyType::Normal;
	float rangedegree_;							//範囲（角度）
	float length_;								//長さ
	float directiondegree_;						//方向（角度）
	float speed_;								//速度
	tnl::Quaternion rot_m_;						//
	
	//------------------------	関数	----------------------------------//
	void Normal(float delta_time);
	void Speed(float delta_time);
	void Power(float delta_time);
	void Chase(float delta_time);
	void ChaseAttack(float delta_time);
	void MoveToPosition(float delta_time);
	bool PlayerLookTrigger(tnl::Vector3 pos);
	tnl::Vector3 RandomPosition(const tnl::Vector3& center, float range);
};