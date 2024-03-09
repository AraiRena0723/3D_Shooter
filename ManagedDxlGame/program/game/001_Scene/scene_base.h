#pragma once

class SceneBase {
public:
	SceneBase() {};
	virtual ~SceneBase() {};
	virtual void update(float delta_time) {};
	virtual void draw() = 0;
protected:
	int now_score = 0;
	int high_score = 0;
	int back_ground_gfx = 0;
	int snd_bgm_hdl = 0;
	int snd_se_bullet = 0;

};

//BGM�Ȃǂ͌ォ��}�l�[�W���[�����
//�V�[�����ׂ��ň����p����悤��