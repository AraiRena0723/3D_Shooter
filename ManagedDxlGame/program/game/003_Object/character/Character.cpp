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
	invincible_timer_ = 0.0f;	//タイマーをリセット
}

//----------------------------------------------------------------------------------------
//	アップデート
//----------------------------------------------------------------------------------------

void Character::setLight()
{
	// ライトの設定
	//自己発光
	DxLib::COLOR_F emissive = { 0.8f,0.8f,0.8f,1 };
	//環境光
	DxLib::COLOR_F ambient = { 1,1,1,1 };
	//拡散光
	DxLib::COLOR_F diffuse = { 0.8f,0.8f,0.8f,1 };
	//メタリック
	DxLib::COLOR_F specular = { 0,0,0,1 };

	MV1SetMaterialEmiColor(model_hdl_, 0, emissive);
	MV1SetMaterialAmbColor(model_hdl_, 0, ambient);
	MV1SetMaterialDifColor(model_hdl_, 0, diffuse);
	MV1SetMaterialSpcColor(model_hdl_, 0, specular);
	// 強いほど光が鋭くなる
	MV1SetMaterialSpcPower(model_hdl_, 0, 0.5f);
}

//----------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------

MATRIX Character::GetTransformMatrix()
{
	// ①拡大縮小を設定する

	// tnl::m_scaleをVECTOR型に変換
	VECTOR vecter_scale = wta::ConvertToVECTOR(scale_);

	// スケールを設定する
	// VECTOR型からMATRIX型に変換
	MATRIX matrix_scale = MGetScale(vecter_scale);

	// ------------------------

	// ②回転を設定する

	// クォータニオンから回転行列を設定する
	MATRIX matrix_rotation = wta::ConvertQuaternionToMatrix(rot_);

	// ------------------------

	// ③座標を設定する

	// tnl::m_posをVECTOR型に変換
	VECTOR vecter_pos = wta::ConvertToVECTOR(pos_);

	// 移動を設定する
	MATRIX matrix_translation = MGetTranslate(vecter_pos);

	// ------------------------

	// ④行列の合成（スケール、回転、平行移動）

	// MMult関数 : 二つの行列の乗算を行う
	MATRIX matrix_transform = MMult(matrix_scale, matrix_rotation);

	// 更に行列の乗算を行う（DXlibでは３つ以上の乗算はないため、分けて計算）
	// 一般的には拡大縮小→回転→平行移動の順序で乗算
	matrix_transform = MMult(matrix_transform, matrix_translation);

	return matrix_transform;
}
