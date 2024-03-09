#include "../../dxlib_ext/dxlib_ext.h"
#include "../000_Manager/game_manager.h"
#include "scene_title.h"
#include "scene_play.h"

SceneTitle::SceneTitle()
{
	snd_bgm_hdl = LoadSoundMem("sound/title.wav");
}

SceneTitle::~SceneTitle()
{
	DeleteSoundMem(snd_bgm_hdl);
}

void SceneTitle::update(float delta_time)
{
	sequence.update(delta_time);
	PlaySoundMem(snd_bgm_hdl, DX_PLAYTYPE_LOOP, false);
}

void SceneTitle::draw()
{
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, back_ground_gfx, true);
	SetFontSize(50);
	DrawStringEx(400, DXE_WINDOW_HEIGHT / 3, -1, "AlienShips shooting");
	DrawStringEx(535, DXE_WINDOW_HEIGHT - 150, -1, "スタート");

}

bool SceneTitle::SeqIdle(float delta_time)
{
	if (sequence.isStart()) {
		back_ground_gfx = LoadGraph("graphics/space-background.jpg");
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		auto mgr = GameManager::GetInstance();
		mgr->ChangeScene(new ScenePlay());
	}
	return true;
}

//配布元「創作堂さくら紅葉」
//ＵＲＬ「https ://yukizakura.net/」