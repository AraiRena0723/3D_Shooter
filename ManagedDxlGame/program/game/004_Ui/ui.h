#pragma once

class Player;
class Enemy;
class PlayerCamera;
struct RaderEnemy
{
	float angle;
	float range;
	bool flag;

};

class Ui {
public:
	Ui(std::weak_ptr<Player>player,std::weak_ptr<Enemy>enemy);
	~Ui();

	void UpDate(float delta_time);
	void Draw(Shared<dxe::Camera>camera);

private:
	//------------------------	�֐�	----------------------------------//
	void DrawPlayerStatus();
	void DrawEnemyStatus();
	void DrawEnemyPos();
	//------------------------	�|�C���^	----------------------------------//
	std::weak_ptr<Player> player_ ;
	std::weak_ptr<Enemy> enemy_ ;
	//------------------------	�ϐ�	----------------------------------//
	int player_hp_hdl;
	int enemy_hp_hdl;
};