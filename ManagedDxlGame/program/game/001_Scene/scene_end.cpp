#include "../../dxlib_ext/dxlib_ext.h"
#include "../000_Manager/game_manager.h"
#include "scene_end.h"
#include "scene_title.h"

SceneEnd::SceneEnd()
{
}

SceneEnd::~SceneEnd()
{
}

void SceneEnd::update(float delta_time)
{
	sequence.update(delta_time);
}

void SceneEnd::draw()
{
}

bool SceneEnd::seqIdle(float delta_time)
{
	if (sequence.isStart()) {
		back_ground_gfx = LoadGraph("graphics/space-background.jpg");

	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		auto mgr = GameManager::GetInstance();
		mgr->ChangeScene(new SceneTitle());
	}
	return true;
}
