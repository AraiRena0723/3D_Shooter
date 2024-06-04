#include "../../../dxlib_ext/dxlib_ext.h"
#include "stage.h"

Stage::Stage()
{
	object_type_ = ObjectType::Stage;
	ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));
	//stage_ = dxe::Mesh::CreateCubeMV(5000, 20, 20);
	stage_ = dxe::Mesh::CreateSphereMV(30000, 20, 20);
	stage_->setTexture(dxe::Texture::CreateFromFile("graphics/space/nebulablue.png"));
	stage_->loadMaterial("material.bin");
}

Stage::~Stage()
{
	
}

void Stage::Update(float delta_time)
{
	stage_->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(0.01f));
	

}

void Stage::Draw(Shared<dxe::Camera> camera)
{
	stage_->render(camera);
}

void Stage::OnHit(Shared<Object> object)
{
	if (object->GetType() == ObjectType::Player) {

	}
	else if (object->GetType() == ObjectType::Enemy) {

	}
	else if (object->GetType() == ObjectType::Bullet) {
		//object->setIsActive(false);
	}
	
}
