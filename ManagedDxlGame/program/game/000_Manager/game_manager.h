#pragma once

class SceneBase;

class GameManager {
public:
	static GameManager* GetInstance(SceneBase* start_scene = nullptr);
	static void Destroy();
	void ChangeScene(SceneBase* next_scene, float trans_time = 0.5f);
	void Update(float delta_time);

private:
	GameManager(SceneBase* start_scene);
	float trans_time_;
	int transition_graph_hdl = 0;

	SceneBase* now_scene_ = nullptr;
	SceneBase* next_scene_ = nullptr;

	tnl::Sequence<GameManager> sequence = tnl::Sequence<GameManager>(this, &GameManager::SeqTransIn);
	bool SeqTransIn(const float delta_time);
	bool SeqTransOut(const float delta_time);
	bool SeqRunScene(const float delta_time);
};