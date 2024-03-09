#pragma once
#include "../../object.h"
#include "../Character.h"

class Camera;
class Character;

class Player : public Character {
public:
	Player();
	~Player();
	void Update(float delta_time) override;
	void Draw(Shared<dxe::Camera> camera) override;

	//------------------------	Getter&Setter	----------------------------------//
	tnl::Vector3 GetPlayerPos() { return pos_; }

private:
	//------------------------	ïœêî	----------------------------------//
	tnl::Sequence<Player> sequence_ = tnl::Sequence<Player>(this, &Player::SeqIdle);
	tnl::Quaternion rot_z_;
	tnl::Quaternion move_rot;
	tnl::Vector3 move_pos_;
	tnl::Vector3 axis;
	float speed_ ;
	float move_ ;
	float move_rotation_ ;
	int player_tupe;
	//------------------------	ä÷êî	----------------------------------//

	bool SeqIdle(const float delta_time);
	bool SeqMove(const float delta_time);
	bool SeqRolling(const float delta_time);
	bool SeqSomersault(const float delta_time);
	bool SeqBoost(const float delta_time);
	bool SeqDeath(const float delta_time);
};