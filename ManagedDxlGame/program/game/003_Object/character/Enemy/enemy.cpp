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

//通常型
void Enemy::Normal(float delta_time)
{
	if (!PlayerLookTrigger(player->GetPlayerPos())) {
		MoveToPosition(delta_time);
	}

}

//スピード型
void Enemy::Speed(float delta_time)
{
	if (!PlayerLookTrigger(player->GetPlayerPos())) {
		MoveToPosition(delta_time);
	}
	else {
		
	}
}

//火力型
void Enemy::Power(float delta_time)
{
	if (!PlayerLookTrigger(player->GetPlayerPos())) {
		MoveToPosition(delta_time);
	}
	else {

	}
}

//追跡（球は打たずに体当たりで攻撃してくる）
void Enemy::Chase(float delta_time)
{
	
	if (!PlayerLookTrigger(player->GetPlayerPos())) {
		MoveToPosition(delta_time);
	}
	else {
		ChaseAttack(delta_time);
	}
}

//プレイヤーに向かって進む
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
	//目的地の設定
	tnl::Vector3 move_position = RandomPosition(pos_, 1000);

	//生成した座標に向かって移動
	//現在位置から目的地までのベクトルを計算
	tnl::Vector3 direction = tnl::Vector3::Normalize(move_position - pos_);
	//移動速度を定義
	float move_speed = 150.0f;
	//時間に応じて移動量を計算
	tnl::Vector3 move_distance = direction * move_speed * delta_time;

	//現在値を更新
	pos_ += move_distance;

	//移動方向から回転を計算
	tnl::Quaternion rotation;
	//上向き単位のベクトルを使用
	tnl::Quaternion identity = tnl::Quaternion::RotationAxis({ 0,1,0 }, 0.0f);
	//Directionが零ベクトルでない場合
	if (direction.length() >= 0.0f) {
		//目的地との回転軸を計算
		tnl::Vector3 rotation_axis = tnl::Vector3::Cross({ 0,0,1 }, direction);
		//回転量を指定してクォータニオンを生成
		rotation = tnl::Quaternion::RotationAxis(rotation_axis, move_speed);
	}

	//プレイヤーの回転を更新
	rot_m_ *= rotation;

	// 向きの補完関数(計算結果は自動でrotへ代入される)
	// arg1 : 向きたい方向
	// arg2 : 向くまでの時間
	rot_.slerp(rot_m_, move_speed);
	// 上記で更新した各パラメータを行列計算
	matrix_ = GetTransformMatrix();
	// 行列が反映されたモデルの描画
	MV1SetMatrix(model_hdl_, matrix_);
}

//扇型の視界　改良
bool Enemy::PlayerLookTrigger(tnl::Vector3 pos)
{
	//キャラと敵のベクトル
	tnl::Vector3 enemy_fan = {
		pos.x -= pos_.x,
		pos.y -= pos_.y,
		pos.z -= pos_.z,
	};
	//ベクトルの長さを計算
	float vec_length = sqrtf((enemy_fan.x * enemy_fan.x) + (enemy_fan.y * enemy_fan.y)+ (enemy_fan.z * enemy_fan.z));
	//ベクトルと扇の長さの比較
	//扇の長さ　<=　敵とキャラの間の長さ
	if (length_ <= vec_length) {
		//当たっていない
		return false;
	}
	else {

		//敵の扇の２等分する線のベクトルを計算
		float direction_rad = tnl::ToRadian(directiondegree_);
		tnl::Vector3 fan_dir = { cosf(direction_rad),sinf(direction_rad),0 };
		//敵とキャラのベクトルを単位ベクトルに変換
		tnl::Vector3 normal_enemy_to_point = {
			enemy_fan.x / vec_length,
			enemy_fan.y / vec_length,
			enemy_fan.z / vec_length,
		};
		// 内積の計算
		float dot = normal_enemy_to_point.x * fan_dir.x + normal_enemy_to_point.y * fan_dir.y + normal_enemy_to_point.z * fan_dir.z;
		//扇の範囲をコサインにする
		float fan_cos = cosf(tnl::ToRadian(rangedegree_ / 2.0f));
		//キャラが敵の視界（扇）の範囲内にあるかを比較する
		//敵の視野角 >=　敵の方向ベクトルと敵とキャラのベクトルのなす角度
		if (fan_cos >= dot) {
			//当たってない
			return false;
		}
		else {
			return true;
		}
	}

}

//ランダム座標の生成
tnl::Vector3 Enemy::RandomPosition(const tnl::Vector3& center, float range)
{
	//ランダムな座標を生成
	float randomX = center.x + static_cast<float> (rand()) / (static_cast<float>(RAND_MAX)) * range + range / 2.0f;
	float randomY = center.y + static_cast<float> (rand()) / (static_cast<float>(RAND_MAX)) * range + range / 2.0f;
	float randomZ = center.z + static_cast<float> (rand()) / (static_cast<float>(RAND_MAX)) * range + range / 2.0f;

	//生成されたランダムな座標を返す
	return tnl::Vector3(randomX, randomY, randomZ);
}
