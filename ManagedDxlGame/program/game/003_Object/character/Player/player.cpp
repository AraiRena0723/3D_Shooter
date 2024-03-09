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
	//���f���̓ǂݍ���
	model_hdl_ = MV1LoadModel("model/plane.mv1");
	//���f���̃e�N�X�`���[
	texture_hdl_ = LoadGraph("model/StarSparrow_Orange.png");
	//���f���Ƀe�N�X�`���[��g�ݍ��킹��
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
	

	// �����̕⊮�֐�(�v�Z���ʂ͎�����rot�֑�������)
	// arg1 : ������������
	// arg2 : �����܂ł̎���
	rot_.slerp(rot_z_, move_rotation_);
	// ��L�ōX�V�����e�p�����[�^���s��v�Z
	matrix_ = GetTransformMatrix();
	// �s�񂪔��f���ꂽ���f���̕`��
	MV1SetMatrix(model_hdl_, matrix_);
}

void Player::Draw(Shared<dxe::Camera> camera)
{
	setLight();
	MV1DrawModel(model_hdl_);
}

//�A�C�h�����
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

//�ړ����
bool Player::SeqMove(const float delta_time)
{
	move_pos_ = { 0,0,0 };
	if (tnl::Input::IsKeyDown(eKeys::KB_W)) {
		//�O�ֈړ�
		move_pos_ = { 0,0,5, };
		rot_z_ = tnl::Quaternion::LookAtAxisY(pos_, pos_ + tnl::Vector3(0, 0, 1));
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_S)) {
		//���ֈړ�
		move_pos_ = { 0,0,-5, };
		rot_z_ = tnl::Quaternion::LookAtAxisY(pos_, pos_ + tnl::Vector3(0, 0, -1));
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_D)) {
		//�E����
		move_pos_ = { 5,0,0, };
		////�v���C���[�̉�]
		rot_z_ = tnl::Quaternion::LookAtAxisY(pos_, pos_ + tnl::Vector3(1, 0, 0));
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_A)) {
		//������
		move_pos_ = { -5,0,0, };
		////�v���C���[�̉�]
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
		//Bullet�̔��ˏ���

	}
	pos_ += move_pos_;
	if (move_pos_.length() < 1.0f) {
		sequence_.change(&Player::SeqIdle);
	}
	DrawStringEx(0, 0, -1, "SeqMove");
	// �����̕⊮�֐�(�v�Z���ʂ͎�����rot�֑�������)
	// arg1 : ������������
	// arg2 : �����܂ł̎���
	rot_.slerp(rot_z_, move_rotation_);
	// ��L�ōX�V�����e�p�����[�^���s��v�Z
	matrix_ = GetTransformMatrix();
	// �s�񂪔��f���ꂽ���f���̕`��
	MV1SetMatrix(model_hdl_, matrix_);
	return true;
}

//���[�����O���(�㉺���E�ړ��\���_���[�W���󂯂Ȃ�)
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

//���S
bool Player::SeqDeath(const float delta_time)
{
	DrawStringEx(0, 0, -1, "SeqDeath");
	return true;
}

//X��180��]��Z��180��]�������ς��
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
