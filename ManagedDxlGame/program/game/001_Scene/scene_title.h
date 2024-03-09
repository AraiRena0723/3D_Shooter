#pragma once
#include "scene_base.h"

class SceneTitle : public SceneBase {
public:
	SceneTitle();
	~SceneTitle() override;
	void update(float delta_time) override;
	void draw()override;

private:
	tnl::Sequence<SceneTitle>sequence = tnl::Sequence<SceneTitle>(this, &SceneTitle::SeqIdle);
	bool SeqIdle(float delta_time);

};