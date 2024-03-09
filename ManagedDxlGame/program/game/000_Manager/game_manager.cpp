#include "../../dxlib_ext/dxlib_ext.h"
#include "../gm_main.h"
#include "game_manager.h"
#include "../001_Scene/scene_base.h"

//�R���X�g���N�^
GameManager::GameManager(SceneBase* start_scene)
	:now_scene_(start_scene) {
	transition_graph_hdl = LoadGraph("graphics/black.bmp");
}

//�V���ɃV�[����ǉ�����
GameManager* GameManager::GetInstance(SceneBase* start_scene)
{
	static GameManager* instance = nullptr;
	if (!instance) {
		instance = new GameManager(start_scene);
	}
	return instance;
}

//�폜�p
void GameManager::Destroy()
{
	delete GetInstance();
}

//�V�[���̐؂�ւ�
void GameManager::ChangeScene(SceneBase* next_scene, float trans_time)
{
	next_scene_ = next_scene;
	trans_time = trans_time;
	sequence.change(&GameManager::SeqTransOut);
}

void GameManager::Update(float delta_time)
{
	if (now_scene_) now_scene_->update(delta_time);
	if (now_scene_) now_scene_->draw();

	sequence.update(delta_time);
}

bool GameManager::SeqTransIn(const float delta_time)
{
	int alpha = 255 - (sequence.getProgressTime() / trans_time_ * 255.0f);
	if (alpha <= 0) {
		sequence.change(&GameManager::SeqRunScene);
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, transition_graph_hdl, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	return true;
}

bool GameManager::SeqTransOut(const float delta_time)
{
	int alpha = (sequence.getProgressTime() / trans_time_ * 255.0f);//�V�[�P���X�̌o�ߎ��ԁ@���@0.5�@�~�@255.0
	if (alpha >= 255) {
		sequence.change(&GameManager::SeqTransIn);                  //gamemanager����transin�ɐ؂�ւ���
		delete now_scene_;
		now_scene_ = nullptr;                                        //now_scene����ɂ���
		now_scene_ = next_scene_;                                    //now_scene��V���ɒǉ�
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, transition_graph_hdl, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	return true;
}

bool GameManager::SeqRunScene(const float delta_time)
{
	return true;
}
