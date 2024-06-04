#include "../../dxlib_ext/dxlib_ext.h"
#include "loadcsv.h"

//Enemy‚ÌCSVî•ñ‚ğ“Ç‚İæ‚é
std::unordered_map<int, EnemyDate> LoadCsv::LoadEnemyDate(const std::string& enemy_csv)
{
	std::unordered_map <int, EnemyDate> e_info;

	auto ecsv = tnl::LoadCsv(enemy_csv);

	for (int i = 0; i < ecsv.size(); i++) {
		if (i == 0) continue;
		EnemyDate date;
		
		date.enemy_type = ecsv[i][0].getInt();
		date.enemy_id = ecsv[i][1].getInt();
		date.enemy_hp = ecsv[i][2].getInt();
		date.enemy_score = ecsv[i][3].getInt();
		date.enemy_speed = ecsv[i][4].getFloat();
		date.enemy_damage = ecsv[i][5].getInt();
		date.enemy_bullet_interval = ecsv[i][6].getInt();
		date.enemy_size = ecsv[i][7].getFloat();
		date.enemy_model = ecsv[i][8].getString();
		date.enemy_texture = ecsv[i][9].getString();

		e_info[date.enemy_id] = date;
		
	}

	return e_info;
}
