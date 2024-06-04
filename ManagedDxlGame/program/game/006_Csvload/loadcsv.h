#pragma once


struct EnemyDate
{
	int enemy_type{};
	int enemy_id{};
	int enemy_hp{};
	int enemy_score{};
	float enemy_speed{};
	int enemy_damage{};
	int enemy_bullet_interval{};
	float enemy_size{};
	std::string enemy_model{};
	std::string enemy_texture{};
};


//CSV‚Ì“Ç‚İ‚İ—pƒNƒ‰ƒX
class LoadCsv {
public:
	
	
	std::unordered_map<int, EnemyDate> LoadEnemyDate(const std::string& enemy_csv);

private:
	
};