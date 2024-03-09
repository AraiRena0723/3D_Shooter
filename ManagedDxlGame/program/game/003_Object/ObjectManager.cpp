#include "../../dxlib_ext/dxlib_ext.h"
#include "ObjectManager.h"
#include "object.h"
#include "character/Character.h"
#include "character/Enemy/enemy.h"
#include "character/Player/player.h"
#include "Bullet/bullet.h"

void ObjectManager::factory()
{

	for (int i = 0; i < 10; i++) {
		enemy_.emplace_back(std::make_shared<Enemy>());
	}
	for (auto enem : enemy_) {
		object_.emplace_back(enem);
	}
}

void ObjectManager::update(float delta_time)
{
	//秒数処理
	auto enem = enemy_.begin();
	while (enem != enemy_.end())
	{
		
		if(!(*enem)->getIsActive()){
			(*enem)->setIsActive(true);
			//spawn処理
		}
		else {
			//デバック用に
		}
		enem++;
	}
}
