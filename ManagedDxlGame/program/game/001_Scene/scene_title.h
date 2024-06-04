#pragma once
#include "scene_base.h"

class PlayerCamera;
class MusicManager;
class ScenePlay;

class SceneTitle : public SceneBase {
public:
	SceneTitle();
	~SceneTitle() override;
	void Update(float delta_time) override;
	void Draw()override;

private:
	tnl::Sequence<SceneTitle>sequence_ = tnl::Sequence<SceneTitle>(this, &SceneTitle::SeqIdle);
	bool SeqIdle(float delta_time);
	
	
};