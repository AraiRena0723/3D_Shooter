#pragma once
#include "music_bgm.h"
#include "music_se.h"

class MusicManager {
public :
	MusicManager(){}

	static MusicManager& GetInstance() {
		static MusicManager instance;
		return instance;
	}
	
	//------------------------	関数	----------------------------------//

	void LoadBGM(const std::string& path);								//BGMの読み込み
	void LoadSE(const std::string& key,const std::string& path);		//効果音の読み込み

	void PlaySE(const std::string& key);
	void StopSE(const std::string& key);

	void PlayBGM();
	void StopBGM();

private:
	std::unique_ptr<MusicBGM> music_bgm;
	std::map<std::string, std::unique_ptr<MusicSE>> music_se;

};