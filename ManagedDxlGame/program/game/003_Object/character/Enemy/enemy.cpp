#include "../../.././../dxlib_ext/dxlib_ext.h"
#include "enemy.h"
#include "../Player/player.h"
#include "../../../001_Scene/scene_play.h"
#include "../../../002_Camera/camera.h"
#include "../../../006_Csvload/loadcsv.h"

//----------------------------------------------------------------------------------------
//	コンストラクタ
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
//	デストラクタ
//----------------------------------------------------------------------------------------

Enemy::~Enemy()
{
	MV1DrawModel(model_hdl_);
}

//----------------------------------------------------------------------------------------
//	アップデート
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
//	描画
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
//	扇型の視界　改良
//  進行方向　(V)  range=0.785
// ----------------------------------------------------------------------------------------

bool Enemy::PlayerLookTrigger(tnl::Vector3 pos)
{
	const float PI = 3.14159265358979323846f;


	//	目的地への方向ベクトルを計算
	tnl::Vector3 move_vector = Vector(move_pos);

	//	現在値を更新
	pos_ += move_vector;

	//	向きたい方向を計算
	//	arg1 : 現在値
	//	arg2 : 目的地
	rot_m_ = tnl::Quaternion::LookAtAxisY(pos_, move_pos);
	
	//	プレイヤーの方向ベクトルを計算
	// arg1 : プレイヤーの座標
	tnl::Vector3 player_vector = Vector(pos);

	//	内積を計算
	// arg1 : 目的地の方向ベクトル
	// arg2 : プレイヤーの方向ベクトル
	float dot_product = InnerProduct(move_vector, player_vector);

	//	アークコサインを計算して、ラジアン単位での角度を求める
	//	arg1 : 内積
	float angle_rad = std::acos(dot_product);

	//	
	float radians = range * (PI / 180.0f);
	// 　指定した角度より大きい場合見えてない
	if (angle_rad > radians) {
		return false;
	}//　指定した角度より小さい場合見えてる
	else {
		//EnemyとPlayerの距離を計算
		// arg1 : 対象の座標
		float distance = Distance(pos);

		//元々設定している距離よりプレイヤーとの距離が近い場合Trueを返す
		if (length_ >= distance) {
			return true;
		}
	}
	return false;


}

//----------------------------------------------------------------------------------------
//	ランダム座標の生成
//	arg1 : 対象の座標
//	arg1 : 範囲
//----------------------------------------------------------------------------------------

tnl::Vector3 Enemy::RandomPosition(const tnl::Vector3& center, float range)
{
	//ランダムな座標を生成
	float randomX = center.x + static_cast<float> (rand()) / (static_cast<float>(RAND_MAX)) * range + range / 2.0f;
	//float randomY = center.y + static_cast<float> (rand()) / (static_cast<float>(RAND_MAX)) * range + range / 2.0f;
	float randomZ = center.z + static_cast<float> (rand()) / (static_cast<float>(RAND_MAX)) * range + range / 2.0f;

	//生成されたランダムな座標を返す
	return tnl::Vector3(randomX, 0.0f, randomZ);
}

//----------------------------------------------------------------------------------------
//	方向のベクトルを計算
//	arg1 : 対象の座標
//----------------------------------------------------------------------------------------

tnl::Vector3 Enemy::Vector(tnl::Vector3 pos)
{
	tnl::Vector3 direction = pos_ - pos;
	direction.normalize();

	return direction;
}

//----------------------------------------------------------------------------------------
//	内積の計算
//	arg1 : 自身から進行方向のベクトル
//	arg2 : 自身から対象に向かうベクトル
//----------------------------------------------------------------------------------------

float Enemy::InnerProduct(tnl::Vector3 vector1, tnl::Vector3 vector2) {

	float inner_product = (vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z);

	return inner_product;
}

//----------------------------------------------------------------------------------------
//	2つの座標から距離を求める
//	arg1 : 対象の座標
//----------------------------------------------------------------------------------------

float Enemy::Distance(const tnl::Vector3& pos) {

	tnl::Vector3 diff = pos_ - pos;
	float diff_ = std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);

	return diff_;
}

//----------------------------------------------------------------------------------------
//	当たり判定
//----------------------------------------------------------------------------------------

void Enemy::OnHit(Shared<Object> object)
{

	//	敵同士はHPは削らないが重ならないようにする
	if (object->GetType() == ObjectType::Player) {

		if (enemytype == EnemyType::Chase) {
			character_hp_ -= object->getDamage();
		}
		////自身のHPも１削るが重ならないようにする
		tnl::Vector3 center = (pos_ + object->getPos()) / 2.0f;
		tnl::Vector3 toA = tnl::Vector3::Normalize(pos_ - object->getPos());
		tnl::Vector3 toB = tnl::Vector3::Normalize(object->getPos() - pos_);
		pos_ = center + (toA * getCollistionRadius());
		object->getPos() = center + (toB * object->getCollistionRadius());

	}
	//	敵同士はHPは削らないが重ならないようにする
	else if (object->GetType() == ObjectType::Stage) {
		
	}
	

}


//----------------------------------------------------------------------------------------
//	タイプ設定
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
//	モデル・テクスチャーのSetter
//----------------------------------------------------------------------------------------

void Enemy::SetEnemyModel(const std::string& model, const std::string& texture)
{
	
	enem_model_ = model;
	model_hdl_ = MV1LoadModel(enem_model_.c_str());
	//モデルのテクスチャー
	texture_hdl_ = LoadGraph(texture.c_str());
	MV1SetTextureGraphHandle(model_hdl_, 0, texture_hdl_, false);

	////モデルの読み込み
	//model_hdl_ = MV1LoadModel("model/bio/bio.mv1");
	////モデルのテクスチャー
	//texture_hdl_ = LoadGraph("model/bio/bio1.png");
	////モデルにテクスチャーを組み合わせる
	//MV1SetTextureGraphHandle(model_hdl_, 0, texture_hdl_, false);

}

//----------------------------------------------------------------------------------------
//	アイドル状態
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
//	通常型
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
		// 向きの補完関数(計算結果は自動でrotへ代入される)
			// arg1 : 向きたい方向
			// arg2 : 向くまでの時間
		rot_.slerp(rot_m_, delta_time);
		// 上記で更新した各パラメータを行列計算
		matrix_ = GetTransformMatrix();
		// 行列が反映されたモデルの描画
		MV1SetMatrix(model_hdl_, matrix_);
	}
	else {
		is_active_ = false;
	}
	return true;

}

//----------------------------------------------------------------------------------------
//	スピード型
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
//	再セット用
//----------------------------------------------------------------------------------------

void Enemy::ReSetEnemy()
{
	setIsActive(true);
	pos_ = RandomPosition(pos_, 500);
	character_hp_ = enemy_reset_hp;
}

//----------------------------------------------------------------------------------------
//	火力型
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
		
		// 向きの補完関数(計算結果は自動でrotへ代入される)
			// arg1 : 向きたい方向
			// arg2 : 向くまでの時間
		rot_.slerp(rot_m_, delta_time);
		// 上記で更新した各パラメータを行列計算
		matrix_ = GetTransformMatrix();
		// 行列が反映されたモデルの描画
		MV1SetMatrix(model_hdl_, matrix_);
		
	}
	else is_active_ = false;
	return true;
}

//----------------------------------------------------------------------------------------
//	追跡（球は打たずに体当たりで攻撃してくる）
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
//	プレイヤーに向かって進む
//----------------------------------------------------------------------------------------

bool Enemy::ChaseAttack(const float delta_time)
{
	if (character_hp_ > 0) {
		auto playerlock = player_.lock();
		if (!playerlock) {
			return false;
		}
		//相対距離の計算
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
//	移動
//----------------------------------------------------------------------------------------

bool Enemy::MoveToPosition(const float delta_time)
{
	if (character_hp_ > 0) {
		auto playerlock = player_.lock();
		if (!PlayerLookTrigger(playerlock->GetPlayerPos())) {
			//目的地の設定
			move_pos = RandomPosition(pos_, 1000);

			//目的地に到達したかの判定
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

				//生成した座標に向かって移動
				//現在位置から目的地までのベクトルを計算
				tnl::Vector3 direction = Vector(move_pos);

				//現在値を更新
				pos_ += direction;

				//向きたい方向を計算
				// arg1 : 現在値
				// arg2 : 目的地
				rot_m_ = tnl::Quaternion::LookAtAxisY(pos_, move_pos);

				// 向きの補完関数(計算結果は自動でrotへ代入される)
				// arg1 : 向きたい方向
				// arg2 : 向くまでの時間
				rot_.slerp(rot_m_, delta_time);
				// 上記で更新した各パラメータを行列計算
				matrix_ = GetTransformMatrix();
				// 行列が反映されたモデルの描画
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
