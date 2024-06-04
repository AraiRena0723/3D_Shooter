#include "../../dxlib_ext/dxlib_ext.h"
#include "ui.h"
#include "../003_Object/character/Player/player.h"
#include "../003_Object/character/Enemy/enemy.h"

Ui::Ui(std::weak_ptr<Player>player, std::weak_ptr<Enemy>enemy)
	:player_(player),enemy_(enemy)
{
	player_hp_hdl = LoadGraph("graphics/orange_circle1.png");
	enemy_hp_hdl = LoadGraph("graphics/orange_circle1.png");
}

Ui::~Ui()
{

}

void Ui::Draw(Shared<dxe::Camera> camera)
{
	DrawPlayerStatus();
	DrawEnemyStatus();
	DrawEnemyPos();

}

void Ui::DrawPlayerStatus()
{
	auto playerlock = player_.lock();
	if (!playerlock) {
		return;
	}
	if (playerlock->GetCharacterHp() <= 0) {
		return;
	}
	DrawCircleGauge(150,150,100,player_hp_hdl);

}

void Ui::DrawEnemyStatus()
{
	auto enemylock = enemy_.lock();
	if (!enemylock) {
		return;
	}
	if (enemylock->GetCharacterHp() <= 0) {
		return;
	}
	DrawCircleGauge(enemylock->GetEnemyPos().x, enemylock->GetEnemyPos().y+50,100,enemy_hp_hdl);
}

void Ui::DrawEnemyPos()
{
	auto enemylock = enemy_.lock();
	if (!enemylock) {
		return;
	}
	if (enemylock->GetCharacterHp() <= 0) {
		return;
	}

}
