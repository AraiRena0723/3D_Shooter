#pragma once
#include "scene_base.h"

class ScenePlay : public SceneBase {
public:
	ScenePlay();
	~ScenePlay()override;
	void update(float delta_time) override;
	void draw()override;

private:
	tnl::Sequence<ScenePlay> sequence_ = tnl::Sequence<ScenePlay>(this, &ScenePlay::seqIdle);
	bool seqIdle(float delta_time);
};