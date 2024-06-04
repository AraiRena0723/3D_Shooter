#pragma once

class Object;
class PlayerCamera;

class SceneBase {
public:
	SceneBase() {};
	virtual ~SceneBase() {};
	virtual void Update(float delta_time) {};
	virtual void Draw() = 0;
protected:
	int now_score = 0;
	int high_score = 0;
	int back_ground_gfx = 0;
	int snd_bgm_hdl = 0;
	int snd_se_bullet = 0;
	std::list<std::shared_ptr<Object>> object_list_;
};

//BGMなどは後からマネージャーを作る
//シーンを跨いで引き継げるように