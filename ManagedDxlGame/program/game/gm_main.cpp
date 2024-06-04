#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"
#include "000_Manager/game_manager.h"
#include "001_Scene/scene_title.h"
#include "005_music/MusicManager.h"
#include "002_Camera/camera.h"
#include "003_Object/character/Player/player.h"

class PlayerCamera;
class Player;

Shared<PlayerCamera> camera = nullptr;
Shared<Player> player = nullptr;


//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {
	srand(time(0));

	SetFogEnable(true);
	SetFogColor(32, 32, 32);
	SetFogStartEnd(500.0f, 1500.0f);
	ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));
	SetBackgroundColor(32, 32, 32);
	GameManager::GetInstance(new SceneTitle());


}


//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {

	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);
	GameManager::GetInstance()->GetInstance()->Update(delta_time);
}

//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {

	GameManager::GetInstance()->Destroy();

}
