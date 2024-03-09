#include "../../../dxlib_ext/dxlib_ext.h"
#include "../object.h"
#include "bullet.h"

Bullet::Bullet()	
{
	size = 20;
	sphere = dxe::Mesh::CreateSphereMV(size);
	textrue = dxe::Texture::CreateFromFile("graphics/red1.bmp");
	sphere->setTexture(textrue);
	
}

Bullet::~Bullet()
{
	sphere.reset();
	textrue.reset();
}

void Bullet::Update(float delta_time)
{
	sphere->pos_ += move_bullet * 30;
	if (!is_active_) { return; }
	else
	{
		if (bullet_type_ == BulletType::Normal) {

		}
	}

}

void Bullet::Draw(Shared<dxe::Camera> camera)
{
	sphere->render(camera);
}

//is_active‚ðfalse‚É‚·‚é
void Bullet::Destroy()
{
}


void Bullet::Spawn(tnl::Vector3 pos, tnl::Quaternion rot, BulletType type)
{
	pos_ = pos;
	rot_ = rot;
	bullet_type_ = type;
}




