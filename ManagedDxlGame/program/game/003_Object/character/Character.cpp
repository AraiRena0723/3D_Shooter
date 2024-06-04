#include "../object.h"
#include "../../../wta_library/wta_Convert.h"
#include "Character.h"

void Character::Update(float delta_time)
{
}

void Character::Draw(Shared<dxe::Camera> camera)
{
}

//----------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------

void Character::SetCharacterInvincible()
{
	invincible_ = true;
	invincible_timer_ = 0.0f;	//�^�C�}�[�����Z�b�g
}

//----------------------------------------------------------------------------------------
//	�A�b�v�f�[�g
//----------------------------------------------------------------------------------------

void Character::setLight()
{
	// ���C�g�̐ݒ�
	//���Ȕ���
	DxLib::COLOR_F emissive = { 0.8f,0.8f,0.8f,1 };
	//����
	DxLib::COLOR_F ambient = { 1,1,1,1 };
	//�g�U��
	DxLib::COLOR_F diffuse = { 0.8f,0.8f,0.8f,1 };
	//���^���b�N
	DxLib::COLOR_F specular = { 0,0,0,1 };

	MV1SetMaterialEmiColor(model_hdl_, 0, emissive);
	MV1SetMaterialAmbColor(model_hdl_, 0, ambient);
	MV1SetMaterialDifColor(model_hdl_, 0, diffuse);
	MV1SetMaterialSpcColor(model_hdl_, 0, specular);
	// �����قǌ����s���Ȃ�
	MV1SetMaterialSpcPower(model_hdl_, 0, 0.5f);
}

//----------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------

MATRIX Character::GetTransformMatrix()
{
	// �@�g��k����ݒ肷��

	// tnl::m_scale��VECTOR�^�ɕϊ�
	VECTOR vecter_scale = wta::ConvertToVECTOR(scale_);

	// �X�P�[����ݒ肷��
	// VECTOR�^����MATRIX�^�ɕϊ�
	MATRIX matrix_scale = MGetScale(vecter_scale);

	// ------------------------

	// �A��]��ݒ肷��

	// �N�H�[�^�j�I�������]�s���ݒ肷��
	MATRIX matrix_rotation = wta::ConvertQuaternionToMatrix(rot_);

	// ------------------------

	// �B���W��ݒ肷��

	// tnl::m_pos��VECTOR�^�ɕϊ�
	VECTOR vecter_pos = wta::ConvertToVECTOR(pos_);

	// �ړ���ݒ肷��
	MATRIX matrix_translation = MGetTranslate(vecter_pos);

	// ------------------------

	// �C�s��̍����i�X�P�[���A��]�A���s�ړ��j

	// MMult�֐� : ��̍s��̏�Z���s��
	MATRIX matrix_transform = MMult(matrix_scale, matrix_rotation);

	// �X�ɍs��̏�Z���s���iDXlib�ł͂R�ȏ�̏�Z�͂Ȃ����߁A�����Čv�Z�j
	// ��ʓI�ɂ͊g��k������]�����s�ړ��̏����ŏ�Z
	matrix_transform = MMult(matrix_transform, matrix_translation);

	return matrix_transform;
}
