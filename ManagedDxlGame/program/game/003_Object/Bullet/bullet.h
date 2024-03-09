#pragma once

class Camera;
class Bullet : public Object {
public:
	Bullet();
	~ Bullet();

	enum class BulletType {
		Normal
	};
	void Update(float delta_time) override;
	void Draw(Shared<dxe::Camera> camera) override;
	void Destroy();
	void Spawn(tnl::Vector3 pos,tnl::Quaternion rot,BulletType type);
//------------------------	getter&setter	----------------------------------//
	BulletType GetBulletType() { return bullet_type_; }
	int GetDamage() { return bullet_attack; }
	
	

private:
//------------------------	�ϐ�	----------------------------------//
	BulletType bullet_type_ = BulletType::Normal;
	//�e�N�X�`���[
	Shared<dxe::Texture> textrue = nullptr;
	//���b�V��
	Shared<dxe::Mesh> sphere = nullptr;
	float size;
	int bullet_attack;
	tnl::Vector3 move_bullet;
//------------------------	�֐�	----------------------------------//

};