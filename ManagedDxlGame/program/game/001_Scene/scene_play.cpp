#include "../../dxlib_ext/dxlib_ext.h"
#include "../000_Manager/game_manager.h"
#include "scene_play.h"

ScenePlay::ScenePlay()
{
}

ScenePlay::~ScenePlay()
{
}

void ScenePlay::update(float delta_time)
{
	sequence_.update(delta_time);
}

void ScenePlay::draw()
{
}

bool ScenePlay::seqIdle(float delta_time)
{
	return false;
}
