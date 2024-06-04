#include "../../dxlib_ext/dxlib_ext.h"
#include "../000_Manager/game_manager.h"
#include "../005_music/MusicManager.h"
#include "scene_end.h"
#include "scene_title.h"

SceneEnd::SceneEnd()
{

}

SceneEnd::~SceneEnd()
{

}

void SceneEnd::Update(float delta_time)
{
	sequence_.update(delta_time);
}

void SceneEnd::Draw()
{
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, back_ground_gfx, true);
	SetFontSize(50);
	DrawStringEx(400, DXE_WINDOW_HEIGHT / 3, -1, "Clear");
	DrawStringEx(535, DXE_WINDOW_HEIGHT - 150, -1, "ƒ^ƒCƒgƒ‹‚Ö");

}

bool SceneEnd::seqIdle(float delta_time)
{
	if (sequence_.isStart()) {
		back_ground_gfx = LoadGraph("graphics/space-scene.jpg");

		MusicManager::GetInstance().LoadBGM("sound/title.wav");				//
		//MusicManager::GetInstance().LoadSE("start","")					//
		MusicManager::GetInstance().PlayBGM();


	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		auto mgr = GameManager::GetInstance();
		mgr->ChangeScene(new SceneTitle);
	}
	return true;
}
