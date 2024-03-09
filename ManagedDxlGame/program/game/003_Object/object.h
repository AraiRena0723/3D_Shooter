#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

class Camera;
class Object {
public:
	Object() {};
	virtual ~Object() {};
	virtual void Update(float delta_time) = 0;
	virtual void Draw(Shared<dxe::Camera> camera) = 0;
	TNL_PROPERTY(bool,IsActive,is_active_);
protected:
	tnl::Vector3 pos_ ;
	tnl::Vector3 scale_ ;
	tnl::Quaternion rot_;
	bool is_active_ = false;
	
};