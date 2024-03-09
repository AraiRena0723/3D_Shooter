#pragma once
#include "../object.h"

class Camera;
class Stage : public Object {
public:
	Stage();
	~Stage();
	void Update(float  delta_time) override;
	void Draw(Shared<dxe::Camera> camera) override;

private:

};