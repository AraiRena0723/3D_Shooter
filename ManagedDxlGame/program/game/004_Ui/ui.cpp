#include "../../dxlib_ext/dxlib_ext.h"
#include "ui.h"
#include "../003_Object/character/Player/player.h"
#include "../003_Object/character/Enemy/enemy.h"

Ui::Ui()
{
	player = std::make_shared<Player>();
	enemy = std::make_shared<Enemy>();
}

Ui::~Ui()
{
}

void Ui::Draw(Shared<dxe::Camera> camera)
{

}

void Ui::DrawPlayerStatus()
{
	if (player->GetCharacterHp() <= 0) {
		return;
	}
	
	//DrawCircleGauge();
}

void Ui::DrawEnemyStatus()
{
}

void Ui::DrawEnemyPos()
{
	
}
