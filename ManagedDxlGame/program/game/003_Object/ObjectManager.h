#pragma once

class Player;
class Enemy;
class Bullet;
class Character;
class Object;

class ObjectManager {
	ObjectManager();
	~ObjectManager();
	void factory();
	void update(float delta_time);
	std::list<Shared<Object>> GetClassObjectList() { return object_; }

private:
	Shared<Player> player_ = nullptr;
	std::list<Shared<Enemy>> enemy_;
	std::list<Shared<Character>> character_;
	std::list<Shared<Object>> object_;
};