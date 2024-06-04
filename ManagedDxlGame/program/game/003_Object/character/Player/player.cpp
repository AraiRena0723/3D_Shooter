#include "../../../../dxlib_ext/dxlib_ext.h"
#include "../../../001_Scene/scene_play.h"
#include "../../../002_Camera/camera.h"
#include "../../object.h"
#include "../Character.h"
#include "player.h"

//----------------------------------------------------------------------------------------
//	�R���X�g���N�^
//----------------------------------------------------------------------------------------

Player::Player()
	:speed_(5.0f),move_(0),move_rotation_(0),move_pos_({0,0,0}),
	player_bullet_flg(false)
{
	collistion_radius_ = 50;
	setIsActive(true);
	object_type_ = ObjectType::Player;
	character_damage_ = 1;
	pos_ = { 0,0,0 };
	character_hp_ = 5;
	scale_ = { 0.25f,0.25f,0.25f };
	setCollistionRadius(25);
	//���f���̓ǂݍ��� 
	model_hdl_ = MV1LoadModel("model/plane.mv1");
	//���f���̃e�N�X�`���[ 
	texture_hdl_ = LoadGraph("model/StarSparrow_Orange.png");
	//���f���Ƀe�N�X�`���[��g�ݍ��킹��
	MV1SetTextureGraphHandle(model_hdl_, 0, texture_hdl_, false);
	
}

//----------------------------------------------------------------------------------------
//	�f�X�g���N�^
//----------------------------------------------------------------------------------------

Player::~Player()
{
	MV1DeleteModel(model_hdl_);
	DeleteGraph(texture_hdl_);

}

//----------------------------------------------------------------------------------------
//	�A�b�v�f�[�g
//----------------------------------------------------------------------------------------

void Player::Update(float delta_time)
{
	move_ = speed_ * delta_time;
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

//----------------------------------------------------------------------------------------
//	�`��
//----------------------------------------------------------------------------------------

void Player::Draw(Shared<dxe::Camera> camera)
{
	setLight();
	MV1DrawModel(model_hdl_);
}

//----------------------------------------------------------------------------------------
//	�����蔻��̏���
//----------------------------------------------------------------------------------------

void Player::OnHit(Shared<Object> object)
{
	//	�����Ń_���[�W����
	if (object->GetType() == ObjectType::Enemy) {

		//�G�l�~�[�ɓ�����ꍇ�͎��g���_���[�W���󂯂�
		//�����莞�Ԗ��G�ɂȂ�悤�ɂ���
		SetCharacterDamage(object->getDamage());
		SetCharacterInvincible();
	}
	
	//	�����Ń_���[�W����
	else if (object->GetType() == ObjectType::Stage) {

		////�X�e�[�W����o�Ȃ��悤�ɂ���
		//tnl::Vector3 center = (pos_ + object->getPos()) / 2.0f;
		//tnl::Vector3 toA = tnl::Vector3::Normalize(pos_ - object->getPos());
		//tnl::Vector3 toB = tnl::Vector3::Normalize(object->getPos() - pos_);
		//pos_ = center + (toA * getCollistionRadius());
		//object->getPos() = center + (toB * object->getCollistionRadius());
	}
}

//----------------------------------------------------------------------------------------
//	�A�C�h�����
//----------------------------------------------------------------------------------------

bool Player::SeqIdle(const float delta_time) {

	if (invincible_) {
		invincible_timer_ += delta_time;
		if (invincible_timer_ >= 3.0f) {
			invincible_ = false;
		}
	}
	move_ = 0;
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_A, eKeys::KB_D, eKeys::KB_W, eKeys::KB_S)) {
		sequence_.change(&Player::SeqMove);
	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_J)) {
		axis = { 0,0,-1 };
		invincible_ = false;
		sequence_.change(&Player::SeqRolling);
	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_F)) {
		axis = { 0,0,1 };
		invincible_ = false;
		sequence_.change(&Player::SeqRolling);
	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE)) {
		;
		if (!scene_play_) { return false; }
		else if (scene_play_) {
			scene_play_->SpawnBullet(pos_, rot_, object_type_, character_damage_);
		}
	}
	DrawStringEx(0, 0, -1, "SeqIdle");
	return true;

}

//----------------------------------------------------------------------------------------
//	�ړ����
//----------------------------------------------------------------------------------------

bool Player::SeqMove(const float delta_time)
{
	move_pos_ = { 0,0,0 };
	if (tnl::Input::IsKeyDown(eKeys::KB_W)) {
		//�O�ֈړ�
		move_pos_ = { 0,0,5, };
		rot_z_ = tnl::Quaternion::LookAtAxisY(pos_, pos_ + tnl::Vector3(0, 0, 1));
	}
	else if (tnl::Input::IsKeyDown(eKeys::KB_S)) {
		//���ֈړ�
		move_pos_ = { 0,0,-5, };
		rot_z_ = tnl::Quaternion::LookAtAxisY(pos_, pos_ + tnl::Vector3(0, 0, -1));
	}
	else if (tnl::Input::IsKeyDown(eKeys::KB_D)) {
		//�E����
		move_pos_ = { 5,0,0, };
		////�v���C���[�̉�]
		//rot_z_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(-5));
		rot_z_ = tnl::Quaternion::LookAtAxisY(pos_, pos_ + tnl::Vector3(1, 0, 0));
	}
	else if (tnl::Input::IsKeyDown(eKeys::KB_A)) {
		//������
		move_pos_ = { -5,0,0, };
		////�v���C���[�̉�]
		//rot_z_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(5));
		rot_z_ = tnl::Quaternion::LookAtAxisY(pos_, pos_ + tnl::Vector3(-1, 0, 0));
	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_J)) {
		axis = { 0,0,-1 };
		sequence_.change(&Player::SeqRolling);
	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_F)) {
		axis = { 0,0,1 };
		sequence_.change(&Player::SeqRolling);
	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE)) {
		;
		if (!scene_play_) { return false; }
		else if (scene_play_) {
			scene_play_->SpawnBullet(pos_, rot_, object_type_, character_damage_);
		}
	}
	pos_ += move_pos_* speed_;
	if (move_pos_.length() < 1.0f) {
		sequence_.change(&Player::SeqIdle);
	}
	DrawStringEx(0, 0, -1, "SeqMove");
	DrawStringEx(0, 100, -1, "X:%f",pos_.x);
	DrawStringEx(0, 150, -1, "Y:%f",pos_.y);
	DrawStringEx(0, 200, -1, "Z:%f",pos_.z);
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

//----------------------------------------------------------------------------------------
//	���[�����O���(�㉺���E�ړ��\���_���[�W���󂯂Ȃ�)
//----------------------------------------------------------------------------------------

bool Player::SeqRolling(const float delta_time)
{
	
	move_ += 30.0f;
	if (move_ >= 720.0f) {
		move_ = 720.0f;

	}
	
	rot_ *= tnl::Quaternion::RotationAxis(axis, tnl::ToRadian(30));

	if (move_ >= 720.0f) {
		invincible_ = false;
		sequence_.change(&Player::SeqIdle);
	}
	DrawStringEx(0, 0, -1, "SeqRolling");
	return true;

}

//----------------------------------------------------------------------------------------
//	���S
//----------------------------------------------------------------------------------------

bool Player::SeqDeath(const float delta_time)
{
	is_active_ = false;
	DrawStringEx(0, 0, -1, "SeqDeath");
	return true;
}

//----------------------------------------------------------------------------------------
//	X��180��]��Z��180��]�������ς��
//----------------------------------------------------------------------------------------

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
