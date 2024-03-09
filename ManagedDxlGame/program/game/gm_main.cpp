#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"
#include "002_Camera/camera.h"
#include "003_Object/character/Player/player.h"

class PlayerCamera;
class Player;

Shared<PlayerCamera> camera = nullptr;
Shared<Player> player = nullptr;


//------------------------------------------------------------------------------------------------------------
// �Q�[���N�����ɂP�x�������s����܂�
void gameStart() {
	srand(time(0));

	SetFogEnable(true);
	SetFogColor(32, 32, 32);
	SetFogStartEnd(500.0f, 1500.0f);
	ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));
	SetBackgroundColor(32, 32, 32);


	camera = std::make_shared<PlayerCamera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
	player = std::make_shared<Player>();

}


//------------------------------------------------------------------------------------------------------------
// ���t���[�����s����܂�
void gameMain(float delta_time) {

	player->Update(delta_time);
	camera->target_ = player->GetPlayerPos();
	camera->update();


	player->Draw(camera);
	DrawGridGround(camera, 50, 20);
	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);

}

//------------------------------------------------------------------------------------------------------------
// �Q�[���I�����ɂP�x�������s����܂�
void gameEnd() {

}
