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
	//------------------------	�|�C���^	----------------------------------//
	Shared<Player> player = nullptr;

	//------------------------	�ϐ�	----------------------------------//
	EnemyType enemytype = EnemyType::Normal;
	float rangedegree_;							//�͈́i�p�x�j
	float length_;								//����
	float directiondegree_;						//�����i�p�x�j
	float speed_;								//���x
	tnl::Quaternion rot_m_;						//
	
	//------------------------	�֐�	----------------------------------//
	void Normal(float delta_time);
	void Speed(float delta_time);
	void Power(float delta_time);
	void Chase(float delta_time);
	void ChaseAttack(float delta_time);
	void MoveToPosition(float delta_time);
	bool PlayerLookTrigger(tnl::Vector3 pos);
	tnl::Vector3 RandomPosition(const tnl::Vector3& center, float range);
};