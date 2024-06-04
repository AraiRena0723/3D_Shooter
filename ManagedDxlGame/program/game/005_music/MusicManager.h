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
	
	//------------------------	�֐�	----------------------------------//

	void LoadBGM(const std::string& path);								//BGM�̓ǂݍ���
	void LoadSE(const std::string& key,const std::string& path);		//���ʉ��̓ǂݍ���

	void PlaySE(const std::string& key);
	void StopSE(const std::string& key);

	void PlayBGM();
	void StopBGM();

private:
	std::unique_ptr<MusicBGM> music_bgm;
	std::map<std::string, std::unique_ptr<MusicSE>> music_se;

};