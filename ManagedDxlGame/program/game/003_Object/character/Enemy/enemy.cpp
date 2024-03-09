#include "../../../../dxlib_ext/dxlib_ext.h"
#include "enemy.h"
#include "../Player/player.h"

Enemy::Enemy()
	:length_(200.0f),rangedegree_(180.0f),directiondegree_(90.0f)
{
	player = std::make_shared<Player>();
	MV1SetTextureGraphHandle(model_hdl_, 0, texture_hdl_, false);
}

Enemy::~Enemy()
{
	MV1DrawModel(model_hdl_);
}

void Enemy::Update(float delta_time)
{
	if (!is_active_) { return; }
	else {
		if (enemytype == EnemyType::Normal) {
			Normal(delta_time);
		}
		else if (enemytype == EnemyType::Power) {
			Power(delta_time);
		}
		else if (enemytype == EnemyType::Chase) {
			Chase(delta_time);
		}
		else if (enemytype == EnemyType::Speed) {
			Speed(delta_time);
		}
	}
}

void Enemy::Draw(Shared<dxe::Camera> camera)
{
	if (!is_active_) { return; }
	else {
		setLight();
		MV1DrawModel(model_hdl_);
	}
}

//�ʏ�^
void Enemy::Normal(float delta_time)
{
	if (!PlayerLookTrigger(player->GetPlayerPos())) {
		MoveToPosition(delta_time);
	}

}

//�X�s�[�h�^
void Enemy::Speed(float delta_time)
{
	if (!PlayerLookTrigger(player->GetPlayerPos())) {
		MoveToPosition(delta_time);
	}
	else {
		
	}
}

//�Η͌^
void Enemy::Power(float delta_time)
{
	if (!PlayerLookTrigger(player->GetPlayerPos())) {
		MoveToPosition(delta_time);
	}
	else {

	}
}

//�ǐՁi���͑ł����ɑ̓�����ōU�����Ă���j
void Enemy::Chase(float delta_time)
{
	
	if (!PlayerLookTrigger(player->GetPlayerPos())) {
		MoveToPosition(delta_time);
	}
	else {
		ChaseAttack(delta_time);
	}
}

//�v���C���[�Ɍ������Đi��
void Enemy::ChaseAttack(float delta_time)
{
	tnl::Vector3 direction = player->GetPlayerPos() - pos_;
	direction.Normalize(direction);
	speed_ = 10.0f;
	pos_ += direction * delta_time * speed_;
	rot_m_ = tnl::Quaternion::LookAtAxisY(pos_, pos_ + direction);

	rot_.slerp(rot_m_, speed_);
	matrix_ = GetTransformMatrix();
	MV1SetMatrix(model_hdl_, matrix_);
}

void Enemy::MoveToPosition(float delta_time)
{
	//�ړI�n�̐ݒ�
	tnl::Vector3 move_position = RandomPosition(pos_, 1000);

	//�����������W�Ɍ������Ĉړ�
	//���݈ʒu����ړI�n�܂ł̃x�N�g�����v�Z
	tnl::Vector3 direction = tnl::Vector3::Normalize(move_position - pos_);
	//�ړ����x���`
	float move_speed = 150.0f;
	//���Ԃɉ����Ĉړ��ʂ��v�Z
	tnl::Vector3 move_distance = direction * move_speed * delta_time;

	//���ݒl���X�V
	pos_ += move_distance;

	//�ړ����������]���v�Z
	tnl::Quaternion rotation;
	//������P�ʂ̃x�N�g�����g�p
	tnl::Quaternion identity = tnl::Quaternion::RotationAxis({ 0,1,0 }, 0.0f);
	//Direction����x�N�g���łȂ��ꍇ
	if (direction.length() >= 0.0f) {
		//�ړI�n�Ƃ̉�]�����v�Z
		tnl::Vector3 rotation_axis = tnl::Vector3::Cross({ 0,0,1 }, direction);
		//��]�ʂ��w�肵�ăN�H�[�^�j�I���𐶐�
		rotation = tnl::Quaternion::RotationAxis(rotation_axis, move_speed);
	}

	//�v���C���[�̉�]���X�V
	rot_m_ *= rotation;

	// �����̕⊮�֐�(�v�Z���ʂ͎�����rot�֑�������)
	// arg1 : ������������
	// arg2 : �����܂ł̎���
	rot_.slerp(rot_m_, move_speed);
	// ��L�ōX�V�����e�p�����[�^���s��v�Z
	matrix_ = GetTransformMatrix();
	// �s�񂪔��f���ꂽ���f���̕`��
	MV1SetMatrix(model_hdl_, matrix_);
}

//��^�̎��E�@����
bool Enemy::PlayerLookTrigger(tnl::Vector3 pos)
{
	//�L�����ƓG�̃x�N�g��
	tnl::Vector3 enemy_fan = {
		pos.x -= pos_.x,
		pos.y -= pos_.y,
		pos.z -= pos_.z,
	};
	//�x�N�g���̒������v�Z
	float vec_length = sqrtf((enemy_fan.x * enemy_fan.x) + (enemy_fan.y * enemy_fan.y)+ (enemy_fan.z * enemy_fan.z));
	//�x�N�g���Ɛ�̒����̔�r
	//��̒����@<=�@�G�ƃL�����̊Ԃ̒���
	if (length_ <= vec_length) {
		//�������Ă��Ȃ�
		return false;
	}
	else {

		//�G�̐�̂Q����������̃x�N�g�����v�Z
		float direction_rad = tnl::ToRadian(directiondegree_);
		tnl::Vector3 fan_dir = { cosf(direction_rad),sinf(direction_rad),0 };
		//�G�ƃL�����̃x�N�g����P�ʃx�N�g���ɕϊ�
		tnl::Vector3 normal_enemy_to_point = {
			enemy_fan.x / vec_length,
			enemy_fan.y / vec_length,
			enemy_fan.z / vec_length,
		};
		// ���ς̌v�Z
		float dot = normal_enemy_to_point.x * fan_dir.x + normal_enemy_to_point.y * fan_dir.y + normal_enemy_to_point.z * fan_dir.z;
		//��͈̔͂��R�T�C���ɂ���
		float fan_cos = cosf(tnl::ToRadian(rangedegree_ / 2.0f));
		//�L�������G�̎��E�i��j�͈͓̔��ɂ��邩���r����
		//�G�̎���p >=�@�G�̕����x�N�g���ƓG�ƃL�����̃x�N�g���̂Ȃ��p�x
		if (fan_cos >= dot) {
			//�������ĂȂ�
			return false;
		}
		else {
			return true;
		}
	}

}

//�����_�����W�̐���
tnl::Vector3 Enemy::RandomPosition(const tnl::Vector3& center, float range)
{
	//�����_���ȍ��W�𐶐�
	float randomX = center.x + static_cast<float> (rand()) / (static_cast<float>(RAND_MAX)) * range + range / 2.0f;
	float randomY = center.y + static_cast<float> (rand()) / (static_cast<float>(RAND_MAX)) * range + range / 2.0f;
	float randomZ = center.z + static_cast<float> (rand()) / (static_cast<float>(RAND_MAX)) * range + range / 2.0f;

	//�������ꂽ�����_���ȍ��W��Ԃ�
	return tnl::Vector3(randomX, randomY, randomZ);
}
