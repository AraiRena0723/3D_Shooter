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
	Ui();
	~Ui();

	void UpDate(float delta_time);
	void Draw(Shared<dxe::Camera>camera);

private:
	void DrawPlayerStatus();
	void DrawEnemyStatus();
	void DrawEnemyPos();

	Shared<Player> player = nullptr;
	Shared<Enemy> enemy = nullptr;
};