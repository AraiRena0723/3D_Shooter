#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

class Camera;
class Object :public std::enable_shared_from_this<Object> {
public:
	Object() {};
	virtual ~Object() {};
	virtual void Update(float delta_time) = 0;
	virtual void Draw(Shared<dxe::Camera> camera) = 0;
	virtual void OnHit(Shared<Object> object) = 0;
	TNL_PROPERTY(bool,IsActive,is_active_);
	TNL_PROPERTY(float, CollistionRadius, collistion_radius_);
	TNL_PROPERTY(tnl::Vector3, Pos, pos_);
	TNL_PROPERTY(tnl::Quaternion,Rot, rot_);
	TNL_PROPERTY(int, Damage, object_damage_);

	enum class ObjectType {
		Player,
		Enemy,
		Bullet,
		Stage
	};

	ObjectType GetType() { return object_type_; }
	


protected:

	tnl::Vector3 pos_ ;
	tnl::Vector3 scale_ ;
	tnl::Quaternion rot_;
	int object_damage_;
	float collistion_radius_;
	bool is_active_ = false;
	ObjectType object_type_;

	
};

// enum class player,enemy,bullet
// bullet{ enum playerB,enemyB}
