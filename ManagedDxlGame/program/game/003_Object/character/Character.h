#pragma once
#include "../object.h"

class ScenePlay;
class Character : public Object {
public:
	Character() {};
	virtual ~Character() { delete scene_play_; };
	virtual void Update(float delta_time) override;
	virtual void Draw(Shared<dxe::Camera> camera) override;

	//------------------------	Getter&Setter	----------------------------------//
	int GetCharacterHp() { return character_hp_; }
	float GetCharacterSize() { return (pos_.x + pos_.y + pos_.z) / 3; }
	void SetCharacterSize(float size) { scale_.x = size; scale_.y = size; scale_.z = size; }
	void SetCharacterDamage(int damage) { if (!invincible_) { character_hp_ -= damage; } }
	void SetCharacterHP(int hp) { character_hp_ = hp; }
	void SetScenePlay(ScenePlay* scene_play) { scene_play_ = scene_play; }
	void SetCharacterInvincible();

protected:
	//------------------------	ïœêî	----------------------------------//
	int character_hp_ = 0;
	int character_damage_ = 0;
	int model_hdl_ = 0;
	int texture_hdl_ = 0;
	bool is_rotating = false;
	bool invincible_ = false;
	float invincible_timer_ = 0.0f;
	//ÉfÉäÅ[ÉgÇµñYÇÍíçà”
	ScenePlay* scene_play_  = nullptr;
	//------------------------	ä÷êî	----------------------------------//
	void setLight();
	MATRIX matrix_;
	MATRIX GetTransformMatrix();
	
};