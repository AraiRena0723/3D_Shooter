#pragma once
#include "scene_base.h"

class PlayerCamera;
class MusicManager;
class SceneEnd :public SceneBase {
public:
	SceneEnd();
	~SceneEnd();
	void Update(float delta_time) override;
	void Draw() override;
private:
	tnl::Sequence<SceneEnd> sequence_ = tnl::Sequence<SceneEnd>(this, &SceneEnd::seqIdle);
	bool seqIdle(float delta_time);
};

//<a href = "https://jp.freepik.com/free-photo/3d-space-scene_1077125.htm#fromView=search&page=1&position=48&uuid=4791da20-bcb0-4a8d-96d7-8fd5adefcd86">’˜ìÒFkjpargeter^o“TFFreepik< / a>