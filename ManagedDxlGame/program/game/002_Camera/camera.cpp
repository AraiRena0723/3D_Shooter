#include "../../dxlib_ext/dxlib_ext.h"
#include "../gm_main.h"
#include "camera.h"

//--------------------------------------------------------
//	プレイヤーの後ろにカメラが追従するタイプ
//--------------------------------------------------------
void PlayerCamera::update()
{
	//左右移動
	if (tnl::Input::IsKeyDown(eKeys::KB_LEFT)) {
		camera_rot *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(-1));
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) {
		camera_rot *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(1));
	}
	tnl::Vector3 fix_pos = target_ + tnl::Vector3::TransformCoord({ 0,0,-500 }, camera_rot);
	pos_ += (fix_pos - pos_) * 0.1f;

	dxe::Camera::update();
}
