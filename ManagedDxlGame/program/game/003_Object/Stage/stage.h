#pragma once
#include "../object.h"

class Camera;
class Stage : public Object {
public:
	Stage();
	~Stage();
	void Update(float  delta_time) override;
	void Draw(Shared<dxe::Camera> camera) override;
	void OnHit(Shared<Object> object) override;

private:
	Shared<dxe::Mesh> stage_ = nullptr;
};