#pragma once
#include "scene_base.h"

class SceneEnd :public SceneBase {
public:
	SceneEnd();
	~SceneEnd();
	void update(float delta_time) override;
	void draw() override;
private:
	tnl::Sequence<SceneEnd> sequence = tnl::Sequence<SceneEnd>(this, &SceneEnd::seqIdle);
	bool seqIdle(float delta_time);
};