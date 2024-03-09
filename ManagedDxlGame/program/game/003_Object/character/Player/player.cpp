#include "../../../../dxlib_ext/dxlib_ext.h"
#include "../../../002_Camera/camera.h"
#include "../../object.h"
#include "../Character.h"
#include "player.h"

Player::Player()
	:speed_(50.0f),move_(0),move_rotation_(0),move_pos_({0,0,0}),player_tupe(1)
{
	pos_ = { 0,0,0 };
	character_hp_ = 5;
	scale_ = { 0.25f,0.25f,0.25f };
	//モデルの読み込み
	model_hdl_ = MV1LoadModel("model/plane.mv1");
	//モデルのテクスチャー
	texture_hdl_ = LoadGraph("model/StarSparrow_Orange.png");
	//モデルにテクスチャーを組み合わせる
	MV1SetTextureGraphHandle(model_hdl_, 0, texture_hdl_, false);
	
}

Player::~Player()
{
	MV1DeleteModel(model_hdl_);
	DeleteGraph(texture_hdl_);

}

void Player::Update(float delta_time)
{
	
	move_rotation_ = 10.0f * delta_time;
	sequence_.update(delta_time);
	rot_z_ = rot_;
	if (tnl::Input::IsKeyDown(eKeys::KB_UP)) {
		pos_.y += move_;
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_DOWN)) {
		pos_.y -= move_;
	}
	

	// 向きの補完関数(計算結果は自動でrotへ代入される)
	// arg1 : 向きたい方向
	// arg2 : 向くまでの時間
	rot_.slerp(rot_z_, move_rotation_);
	// 上記で更新した各パラメータを行列計算
	matrix_ = GetTransformMatrix();
	// 行列が反映されたモデルの描画
	MV1SetMatrix(model_hdl_, matrix_);
}

void Player::Draw(Shared<dxe::Camera> camera)
{
	setLight();
	MV1DrawModel(model_hdl_);
}

//アイドル状態
bool Player::SeqIdle(const float delta_time) {

	move_ = 0;
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_A, eKeys::KB_D, eKeys::KB_W, eKeys::KB_S)) {
		sequence_.change(&Player::SeqMove);
	}
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_J)) {
		axis = { 0,0,-1 };
		invincible_ = true;
		sequence_.change(&Player::SeqRolling);
	}
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_F)) {
		axis = { 0,0,1 };
		invincible_ = true;
		sequence_.change(&Player::SeqRolling);
	}
	DrawStringEx(0, 0, -1, "SeqIdle");
	return true;
}

//移動状態
bool Player::SeqMove(const float delta_time)
{
	move_pos_ = { 0,0,0 };
	if (tnl::Input::IsKeyDown(eKeys::KB_W)) {
		//前へ移動
		move_pos_ = { 0,0,5, };
		rot_z_ = tnl::Quaternion::LookAtAxisY(pos_, pos_ + tnl::Vector3(0, 0, 1));
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_S)) {
		//後ろへ移動
		move_pos_ = { 0,0,-5, };
		rot_z_ = tnl::Quaternion::LookAtAxisY(pos_, pos_ + tnl::Vector3(0, 0, -1));
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_D)) {
		//右旋回
		move_pos_ = { 5,0,0, };
		////プレイヤーの回転
		rot_z_ = tnl::Quaternion::LookAtAxisY(pos_, pos_ + tnl::Vector3(1, 0, 0));
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_A)) {
		//左旋回
		move_pos_ = { -5,0,0, };
		////プレイヤーの回転
		rot_z_ = tnl::Quaternion::LookAtAxisY(pos_, pos_ + tnl::Vector3(-1, 0, 0));

	}
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_J)) {
		axis = { 0,0,-1 };
		sequence_.change(&Player::SeqRolling);
	}
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_F)) {
		axis = { 0,0,1 };
		sequence_.change(&Player::SeqRolling);
	}
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE)) {
		//Bulletの発射処理

	}
	pos_ += move_pos_;
	if (move_pos_.length() < 1.0f) {
		sequence_.change(&Player::SeqIdle);
	}
	DrawStringEx(0, 0, -1, "SeqMove");
	// 向きの補完関数(計算結果は自動でrotへ代入される)
	// arg1 : 向きたい方向
	// arg2 : 向くまでの時間
	rot_.slerp(rot_z_, move_rotation_);
	// 上記で更新した各パラメータを行列計算
	matrix_ = GetTransformMatrix();
	// 行列が反映されたモデルの描画
	MV1SetMatrix(model_hdl_, matrix_);
	return true;
}

//ローリング状態(上下左右移動可能かつダメージを受けない)
bool Player::SeqRolling(const float delta_time)
{
	
	move_ += 30.0f;
	if (move_ >= 720.0f) {
		move_ = 720.0f;

	}
	
	rot_ *= tnl::Quaternion::RotationAxis(axis, tnl::ToRadian(30));

	MV1SetMatrix(model_hdl_, matrix_);
	if (move_ >= 720.0f) {
		invincible_ = false;
		sequence_.change(&Player::SeqIdle);
	}
	DrawStringEx(0, 0, -1, "SeqRolling");
	return true;

}

//死亡
bool Player::SeqDeath(const float delta_time)
{
	DrawStringEx(0, 0, -1, "SeqDeath");
	return true;
}

//X軸180回転＆Z軸180回転向きが変わる
bool Player::SeqSomersault(const float delta_time)
{
	move_ += 20.0f;
	if (move_ >= 180.0f) {
		move_ = 180.0f;

	}
	rot_ *= tnl::Quaternion::RotationAxis(axis, tnl::ToRadian(30));

	if (move_ >= 180.0f) {

		sequence_.change(&Player::SeqIdle);
	}
	DrawStringEx(0, 0, -1, "SeqSomersault");
	return true;
}
