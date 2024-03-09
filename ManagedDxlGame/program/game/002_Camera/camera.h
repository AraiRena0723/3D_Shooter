#pragma once

class PlayerCamera : public dxe::Camera
{
public:
	~PlayerCamera() {};
	PlayerCamera(int screen_w, int screen_h) :dxe::Camera(screen_w, screen_h) {}

	tnl::Quaternion camera_rot = { 0,0,1,0 };
	float pos_z = 0;
	void update();
	float move_rotation = 10.0f;

};