#pragma once
#include "../object.h"

class Character : public Object {
public:
	Character() {};
	virtual ~Character() {};
	virtual void Update(float delta_time) override;
	virtual void Draw(Shared<dxe::Camera> camera) override;

	//------------------------	Getter&Setter	----------------------------------//
	int GetCharacterHp() { return character_hp_; }
	float GetCharacterSize() { return (pos_.x + pos_.y + pos_.x) / 3; }
	void SetCharacterHp(int damage) { if (!invincible_) { character_hp_ - damage; } }

protected:
	//------------------------	ïœêî	----------------------------------//
	int character_hp_;
	int model_hdl_ = 0;
	int texture_hdl_ = 0;
	bool invincible_ = false;
	//------------------------	ä÷êî	----------------------------------//
	void setLight();
	MATRIX matrix_;
	MATRIX GetTransformMatrix();
	
};