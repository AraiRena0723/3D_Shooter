#pragma once

class Enemy;
class Camera;
class Bullet : public Object {
public:
	Bullet();
	~ Bullet();

	enum class BulletType {
		Normal,
		Tracking
	};
	enum class CharacterType {
		None,
		Player,
		Enemy
	};
	void Update(float delta_time) override;
	void Draw(Shared<dxe::Camera> camera) override;
	void OnHit(Shared<Object> object) override;
	void Destroy();


//------------------------	getter&setter	----------------------------------//
	BulletType GetBulletType() { return bullet_type_; }
	void Spawn(tnl::Vector3 pos,tnl::Quaternion rot,ObjectType type,int damage);			
	
	

private:
//------------------------	変数	----------------------------------//
	tnl::Sequence<Bullet> sequence_b_ = tnl::Sequence<Bullet>(this, &Bullet::SeqIdle);
	BulletType bullet_type_ = BulletType::Normal;							//
	CharacterType character_type = CharacterType::None;						//
	Shared<dxe::Texture> textrue = nullptr;									//テクスチャー
	Shared<dxe::Mesh> sphere = nullptr;										//メッシュ
	float size_;
	float speed_;
	float time_;
	tnl::Vector3 move_bullet_ = { 0,0,0 };
	tnl::Quaternion move_rot = { 0,0,0,0 };
//------------------------	関数	----------------------------------//
	bool SeqIdle(const float delta_time);
	bool NomalShot(const float delta_time);
	bool TrackingShot(const float delta_time);
};