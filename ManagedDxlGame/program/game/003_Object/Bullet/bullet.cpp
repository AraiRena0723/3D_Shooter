#include "../../../dxlib_ext/dxlib_ext.h"
#include "../object.h"
#include "bullet.h"
#include "../character/Enemy/enemy.h"
#include "../character/Player/player.h"

//----------------------------------------------------------------------------------------
//	コンストラクタ
//----------------------------------------------------------------------------------------

Bullet::Bullet()
	:time_(0),speed_(30)
{
	collistion_radius_ = 20.0f;
	is_active_ = false;
	object_type_ = ObjectType::Bullet;
	size_ = 20;
	setCollistionRadius(20);
	sphere = dxe::Mesh::CreateSphereMV(size_);
	textrue = dxe::Texture::CreateFromFile("graphics/red1.bmp");
	sphere->setTexture(textrue);
	
}

//----------------------------------------------------------------------------------------
//	デストラクタ
//----------------------------------------------------------------------------------------

Bullet::~Bullet()
{
	sphere.reset();
	textrue.reset();
}

//----------------------------------------------------------------------------------------
//	アップデート
//----------------------------------------------------------------------------------------

void Bullet::Update(float delta_time)
{

	time_ += delta_time;
	if (!is_active_) { return; }
	else
	{
		sequence_b_.update(delta_time);
	}

}

//----------------------------------------------------------------------------------------
//	描画
//----------------------------------------------------------------------------------------

void Bullet::Draw(Shared<dxe::Camera> camera)
{
	if (!is_active_) { return; }
	else {
		sphere->render(camera);
	}
}

//----------------------------------------------------------------------------------------
//	当たった時の処理
//----------------------------------------------------------------------------------------

void Bullet::OnHit(Shared<Object> object)
{
	//	オブジェクトがエネミー
	if (object->GetType() == ObjectType::Enemy) {

		std::shared_ptr<Enemy> enemy_ = std::dynamic_pointer_cast<Enemy>(object);
		//	自身がエネミーから発射された球の場合
		if (character_type == CharacterType::Enemy) {
			setIsActive(true);
			return;
		}
		//	自身がプレイヤーから発射された球の場合
		else if (character_type == CharacterType::Player) {
			enemy_->SetCharacterDamage(getDamage());
  			
			setIsActive(true);
			return;
		}
	}

}

//----------------------------------------------------------------------------------------
//	Bulletを中身をリセットする
//----------------------------------------------------------------------------------------
void Bullet::Destroy()
{
	is_active_ = false;
	sphere->pos_ = { 0,0,0 };
	setDamage(0);
}

//----------------------------------------------------------------------------------------
//	発射時の設定　（座標/回転/タイプ）
//----------------------------------------------------------------------------------------
void Bullet::Spawn(tnl::Vector3 pos, tnl::Quaternion rot,ObjectType type,int damage)
{
	
	if (type == ObjectType::Player) {
		character_type = CharacterType::Player;
		bullet_type_ = BulletType::Normal;
		time_ = 0.0f;
		move_bullet_ = tnl::Vector3::TransformCoord({ 0,0,1 }, rot);
		sphere->pos_ = pos + move_bullet_ * 200;
		setDamage(damage);
		is_active_ = true;
		return;

	}
	else if (type == ObjectType::Enemy) {
		character_type = CharacterType::Enemy;
		bullet_type_ = BulletType::Normal;
		time_ = 0.0f;
		move_bullet_ = tnl::Vector3::TransformCoord({ 0,0,1 }, rot);
		sphere->pos_ = pos + move_bullet_ * 200;
		setDamage(damage);
		is_active_ = true;
		return;
	}

}

//----------------------------------------------------------------------------------------
//	アイドル状態
//----------------------------------------------------------------------------------------

bool Bullet::SeqIdle(const float delta_time)
{
	if (bullet_type_ == BulletType::Normal) {
		sequence_b_.change(&Bullet::NomalShot);
	}
	else if (bullet_type_ == BulletType::Tracking) {
		sequence_b_.change(&Bullet::TrackingShot);
	}
	return true;
}

//----------------------------------------------------------------------------------------
//	発射の種類　-ノーマル-
//----------------------------------------------------------------------------------------

bool Bullet::NomalShot(const float delta_time)
{	

	sphere->pos_ += move_bullet_ * speed_;
	if (time_ >= 5.0f) {
		Destroy();
	}
	return true;

}

//----------------------------------------------------------------------------------------
//　発射の種類　-トラッキング-
//----------------------------------------------------------------------------------------

bool Bullet::TrackingShot(const float delta_time)
{
	
	
	sphere->pos_ += move_bullet_ * speed_;
	if (time_ >= 5.0f) {
		Destroy();
	}
	return true;
}




