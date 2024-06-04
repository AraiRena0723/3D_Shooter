#include "../../.././../dxlib_ext/dxlib_ext.h"
#include "enemy.h"
#include "../Player/player.h"
#include "../../../001_Scene/scene_play.h"
#include "../../../002_Camera/camera.h"
#include "../../../006_Csvload/loadcsv.h"

//----------------------------------------------------------------------------------------
//	�R���X�g���N�^
//----------------------------------------------------------------------------------------


Enemy::Enemy(std::weak_ptr<Player>player)
	:length_(200.0f),rangedegree_(180.0f),bullet_interbal(0.0f),
	bullet_shoot(0),enemy_bullet_flag(false)
{
	invincible_ = false;
	collistion_radius_ = 20.0f;
	player_ = player;
	object_type_ = ObjectType::Enemy;
	pos_ = RandomPosition({0,0,0},1000);
	

}

//----------------------------------------------------------------------------------------
//	�f�X�g���N�^
//----------------------------------------------------------------------------------------

Enemy::~Enemy()
{
	MV1DrawModel(model_hdl_);
}

//----------------------------------------------------------------------------------------
//	�A�b�v�f�[�g
//----------------------------------------------------------------------------------------

void Enemy::Update(float delta_time)
{
	if (!is_active_) { return; }
	else {
		if (character_hp_ > 0) {
			rot_m_ = rot_;
			bullet_interbal += delta_time;
			sequence_m_.update(delta_time);
		}
		else is_active_ = false;
	}
}

//----------------------------------------------------------------------------------------
//	�`��
//----------------------------------------------------------------------------------------

void Enemy::Draw(Shared<dxe::Camera> camera)
{
	if (!is_active_) { return; }
	else {
		setLight();
		MV1DrawModel(model_hdl_);
		
	}
}

//----------------------------------------------------------------------------------------
//	��^�̎��E�@����
//  �i�s�����@(V)  range=0.785
// ----------------------------------------------------------------------------------------

bool Enemy::PlayerLookTrigger(tnl::Vector3 pos)
{
	const float PI = 3.14159265358979323846f;


	//	�ړI�n�ւ̕����x�N�g�����v�Z
	tnl::Vector3 move_vector = Vector(move_pos);

	//	���ݒl���X�V
	pos_ += move_vector;

	//	���������������v�Z
	//	arg1 : ���ݒl
	//	arg2 : �ړI�n
	rot_m_ = tnl::Quaternion::LookAtAxisY(pos_, move_pos);
	
	//	�v���C���[�̕����x�N�g�����v�Z
	// arg1 : �v���C���[�̍��W
	tnl::Vector3 player_vector = Vector(pos);

	//	���ς��v�Z
	// arg1 : �ړI�n�̕����x�N�g��
	// arg2 : �v���C���[�̕����x�N�g��
	float dot_product = InnerProduct(move_vector, player_vector);

	//	�A�[�N�R�T�C�����v�Z���āA���W�A���P�ʂł̊p�x�����߂�
	//	arg1 : ����
	float angle_rad = std::acos(dot_product);

	//	
	float radians = range * (PI / 180.0f);
	// �@�w�肵���p�x���傫���ꍇ�����ĂȂ�
	if (angle_rad > radians) {
		return false;
	}//�@�w�肵���p�x��菬�����ꍇ�����Ă�
	else {
		//Enemy��Player�̋������v�Z
		// arg1 : �Ώۂ̍��W
		float distance = Distance(pos);

		//���X�ݒ肵�Ă��鋗�����v���C���[�Ƃ̋������߂��ꍇTrue��Ԃ�
		if (length_ >= distance) {
			return true;
		}
	}
	return false;


}

//----------------------------------------------------------------------------------------
//	�����_�����W�̐���
//	arg1 : �Ώۂ̍��W
//	arg1 : �͈�
//----------------------------------------------------------------------------------------

tnl::Vector3 Enemy::RandomPosition(const tnl::Vector3& center, float range)
{
	//�����_���ȍ��W�𐶐�
	float randomX = center.x + static_cast<float> (rand()) / (static_cast<float>(RAND_MAX)) * range + range / 2.0f;
	//float randomY = center.y + static_cast<float> (rand()) / (static_cast<float>(RAND_MAX)) * range + range / 2.0f;
	float randomZ = center.z + static_cast<float> (rand()) / (static_cast<float>(RAND_MAX)) * range + range / 2.0f;

	//�������ꂽ�����_���ȍ��W��Ԃ�
	return tnl::Vector3(randomX, 0.0f, randomZ);
}

//----------------------------------------------------------------------------------------
//	�����̃x�N�g�����v�Z
//	arg1 : �Ώۂ̍��W
//----------------------------------------------------------------------------------------

tnl::Vector3 Enemy::Vector(tnl::Vector3 pos)
{
	tnl::Vector3 direction = pos_ - pos;
	direction.normalize();

	return direction;
}

//----------------------------------------------------------------------------------------
//	���ς̌v�Z
//	arg1 : ���g����i�s�����̃x�N�g��
//	arg2 : ���g����ΏۂɌ������x�N�g��
//----------------------------------------------------------------------------------------

float Enemy::InnerProduct(tnl::Vector3 vector1, tnl::Vector3 vector2) {

	float inner_product = (vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z);

	return inner_product;
}

//----------------------------------------------------------------------------------------
//	2�̍��W���狗�������߂�
//	arg1 : �Ώۂ̍��W
//----------------------------------------------------------------------------------------

float Enemy::Distance(const tnl::Vector3& pos) {

	tnl::Vector3 diff = pos_ - pos;
	float diff_ = std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);

	return diff_;
}

//----------------------------------------------------------------------------------------
//	�����蔻��
//----------------------------------------------------------------------------------------

void Enemy::OnHit(Shared<Object> object)
{

	//	�G���m��HP�͍��Ȃ����d�Ȃ�Ȃ��悤�ɂ���
	if (object->GetType() == ObjectType::Player) {

		if (enemytype == EnemyType::Chase) {
			character_hp_ -= object->getDamage();
		}
		////���g��HP���P��邪�d�Ȃ�Ȃ��悤�ɂ���
		tnl::Vector3 center = (pos_ + object->getPos()) / 2.0f;
		tnl::Vector3 toA = tnl::Vector3::Normalize(pos_ - object->getPos());
		tnl::Vector3 toB = tnl::Vector3::Normalize(object->getPos() - pos_);
		pos_ = center + (toA * getCollistionRadius());
		object->getPos() = center + (toB * object->getCollistionRadius());

	}
	//	�G���m��HP�͍��Ȃ����d�Ȃ�Ȃ��悤�ɂ���
	else if (object->GetType() == ObjectType::Stage) {
		
	}
	

}


//----------------------------------------------------------------------------------------
//	�^�C�v�ݒ�
//----------------------------------------------------------------------------------------

void Enemy::SetEnemyType(int type)
{
	if (type == 0) {
		enemytype = EnemyType::Normal;
	}
	else if (type == 1) {
		enemytype = EnemyType::Speed;
	}
	else if (type == 2) {
		enemytype = EnemyType::Power;
	}
	else if (type == 3) {
		enemytype = EnemyType::Chase;
	}
	else enemytype = EnemyType::Normal;
}

//----------------------------------------------------------------------------------------
//	���f���E�e�N�X�`���[��Setter
//----------------------------------------------------------------------------------------

void Enemy::SetEnemyModel(const std::string& model, const std::string& texture)
{
	
	enem_model_ = model;
	model_hdl_ = MV1LoadModel(enem_model_.c_str());
	//���f���̃e�N�X�`���[
	texture_hdl_ = LoadGraph(texture.c_str());
	MV1SetTextureGraphHandle(model_hdl_, 0, texture_hdl_, false);

	////���f���̓ǂݍ���
	//model_hdl_ = MV1LoadModel("model/bio/bio.mv1");
	////���f���̃e�N�X�`���[
	//texture_hdl_ = LoadGraph("model/bio/bio1.png");
	////���f���Ƀe�N�X�`���[��g�ݍ��킹��
	//MV1SetTextureGraphHandle(model_hdl_, 0, texture_hdl_, false);

}

//----------------------------------------------------------------------------------------
//	�A�C�h�����
//----------------------------------------------------------------------------------------

bool Enemy::SeqIdle(const float delta_time)
{

	if (enemytype == EnemyType::Normal) {
		sequence_m_.change(&Enemy::Normal);
	}
	else if (enemytype == EnemyType::Power) {
		sequence_m_.change(&Enemy::Power);
	}
	else if (enemytype == EnemyType::Chase) {
		sequence_m_.change(&Enemy::Chase);
	}
	else if (enemytype == EnemyType::Speed) {
		sequence_m_.change(&Enemy::Speed);
	}
	return true;
}




//----------------------------------------------------------------------------------------
//	�ʏ�^
//----------------------------------------------------------------------------------------

bool Enemy::Normal(const float delta_time)
{
	auto playerlock = player_.lock();
	if (!playerlock && !scene_play_) {
		return false;
	}
	if (character_hp_ > 0) {

		
		if (PlayerLookTrigger(playerlock->GetPlayerPos())) {
			DrawStringEx(0, 250, -1, "LoolTrigger");
			if (bullet_interbal >= 5) {
				scene_play_->SpawnBullet(pos_, rot_, object_type_, character_damage_);
				rot_.slerp(rot_m_, speed_);
				matrix_ = GetTransformMatrix();
				MV1SetMatrix(model_hdl_, matrix_);
			}
			else bullet_interbal = 0;
		}
		
		else sequence_m_.change(&Enemy::MoveToPosition);
		// �����̕⊮�֐�(�v�Z���ʂ͎�����rot�֑�������)
			// arg1 : ������������
			// arg2 : �����܂ł̎���
		rot_.slerp(rot_m_, delta_time);
		// ��L�ōX�V�����e�p�����[�^���s��v�Z
		matrix_ = GetTransformMatrix();
		// �s�񂪔��f���ꂽ���f���̕`��
		MV1SetMatrix(model_hdl_, matrix_);
	}
	else {
		is_active_ = false;
	}
	return true;

}

//----------------------------------------------------------------------------------------
//	�X�s�[�h�^
//----------------------------------------------------------------------------------------

bool Enemy::Speed(const float delta_time)
{
	auto playerlock = player_.lock();
	
	if (!playerlock && !scene_play_) {
		return false;
	}
	if (character_hp_ > 0){

		if (PlayerLookTrigger(playerlock->GetPlayerPos())) {
			DrawStringEx(0, 250, -1, "LoolTrigger");
			if (bullet_interbal >= 5) {
				scene_play_->SpawnBullet(pos_, rot_, object_type_, character_damage_);
				rot_.slerp(rot_m_, speed_);
				matrix_ = GetTransformMatrix();
				MV1SetMatrix(model_hdl_, matrix_);
			}
			else bullet_interbal = 0;

		}
		sequence_m_.change(&Enemy::MoveToPosition);
	}
	else {
		is_active_ = false;
	}
	return true;
}

//----------------------------------------------------------------------------------------
//	�ăZ�b�g�p
//----------------------------------------------------------------------------------------

void Enemy::ReSetEnemy()
{
	setIsActive(true);
	pos_ = RandomPosition(pos_, 500);
	character_hp_ = enemy_reset_hp;
}

//----------------------------------------------------------------------------------------
//	�Η͌^
//----------------------------------------------------------------------------------------

bool Enemy::Power(const float delta_time)
{
	auto playerlock = player_.lock();
	if (!playerlock && !scene_play_) {
		return false;
	}
	if (character_hp_ > 0) {

		
		if (PlayerLookTrigger(playerlock->GetPlayerPos())) {
			DrawStringEx(0, 250, -1, "LoolTrigger");
			if (bullet_interbal >= 5) {
				scene_play_->SpawnBullet(pos_, rot_, object_type_, character_damage_);
				rot_.slerp(rot_m_, speed_);
				matrix_ = GetTransformMatrix();
				MV1SetMatrix(model_hdl_, matrix_);
			}
			else bullet_interbal = 0;
		}
		sequence_m_.change(&Enemy::MoveToPosition);
		
		// �����̕⊮�֐�(�v�Z���ʂ͎�����rot�֑�������)
			// arg1 : ������������
			// arg2 : �����܂ł̎���
		rot_.slerp(rot_m_, delta_time);
		// ��L�ōX�V�����e�p�����[�^���s��v�Z
		matrix_ = GetTransformMatrix();
		// �s�񂪔��f���ꂽ���f���̕`��
		MV1SetMatrix(model_hdl_, matrix_);
		
	}
	else is_active_ = false;
	return true;
}

//----------------------------------------------------------------------------------------
//	�ǐՁi���͑ł����ɑ̓�����ōU�����Ă���j
//----------------------------------------------------------------------------------------

bool Enemy::Chase(const float delta_time)
{
	auto playerlock = player_.lock();

	if (!playerlock && !scene_play_) {
		return false;
	}
	if (character_hp_ > 0) {

		if (PlayerLookTrigger(playerlock->GetPlayerPos())) {

			sequence_m_.change(&Enemy::ChaseAttack);
		}

		else sequence_m_.change(&Enemy::MoveToPosition);		
	}
	else is_active_ = false;
	return true;
}

//----------------------------------------------------------------------------------------
//	�v���C���[�Ɍ������Đi��
//----------------------------------------------------------------------------------------

bool Enemy::ChaseAttack(const float delta_time)
{
	if (character_hp_ > 0) {
		auto playerlock = player_.lock();
		if (!playerlock) {
			return false;
		}
		//���΋����̌v�Z
		tnl::Vector3 direction = playerlock->GetPlayerPos() - pos_;
		direction.Normalize(direction);

		pos_ += direction * speed_ * delta_time;

		rot_.slerp(rot_m_, speed_);
		matrix_ = GetTransformMatrix();
		MV1SetMatrix(model_hdl_, matrix_);
	}
	else is_active_ = false;
	return true;
}

//----------------------------------------------------------------------------------------
//	�ړ�
//----------------------------------------------------------------------------------------

bool Enemy::MoveToPosition(const float delta_time)
{
	if (character_hp_ > 0) {
		auto playerlock = player_.lock();
		if (!PlayerLookTrigger(playerlock->GetPlayerPos())) {
			//�ړI�n�̐ݒ�
			move_pos = RandomPosition(pos_, 1000);

			//�ړI�n�ɓ��B�������̔���
			if (Distance(move_pos) <= 10.0f) {

				if (enemytype == EnemyType::Normal) {
					sequence_m_.change(&Enemy::Normal);
				}
				else if (enemytype == EnemyType::Speed) {
					sequence_m_.change(&Enemy::Speed);
				}
				else if (enemytype == EnemyType::Power) {
					sequence_m_.change(&Enemy::Power);
				}
				else if (enemytype == EnemyType::Chase) {
					sequence_m_.change(&Enemy::Chase);
				}
			}
			else {

				//�����������W�Ɍ������Ĉړ�
				//���݈ʒu����ړI�n�܂ł̃x�N�g�����v�Z
				tnl::Vector3 direction = Vector(move_pos);

				//���ݒl���X�V
				pos_ += direction;

				//���������������v�Z
				// arg1 : ���ݒl
				// arg2 : �ړI�n
				rot_m_ = tnl::Quaternion::LookAtAxisY(pos_, move_pos);

				// �����̕⊮�֐�(�v�Z���ʂ͎�����rot�֑�������)
				// arg1 : ������������
				// arg2 : �����܂ł̎���
				rot_.slerp(rot_m_, delta_time);
				// ��L�ōX�V�����e�p�����[�^���s��v�Z
				matrix_ = GetTransformMatrix();
				// �s�񂪔��f���ꂽ���f���̕`��
				MV1SetMatrix(model_hdl_, matrix_);
			}
		}
		else {
			if (enemytype == EnemyType::Normal) {
				sequence_m_.change(&Enemy::Normal);
			}
			else if (enemytype == EnemyType::Speed) {
				sequence_m_.change(&Enemy::Speed);
			}
			else if (enemytype == EnemyType::Power) {
				sequence_m_.change(&Enemy::Power);
			}
			else if (enemytype == EnemyType::Chase) {
				sequence_m_.change(&Enemy::Chase);
			}
		}
	}
	else is_active_ = false;
	return true;
}
