#include "../../dxlib_ext/dxlib_ext.h"
#include "MusicManager.h"

void MusicManager::LoadBGM(const std::string& path)
{
	music_bgm = std::make_unique<MusicBGM>(path);
}

void MusicManager::LoadSE(const std::string& key,const std::string& path)
{
	music_se[key] = std::make_unique<MusicSE>(path);
}

void MusicManager::PlaySE(const std::string& key)
{
	auto it = music_se.find(key);
	if (it != music_se.end()) {
		it->second->Play();
	}
}

void MusicManager::StopSE(const std::string& key)
{
	auto it = music_se.find(key);
	if (it != music_se.end()) {
		it->second->Stop();
	}
}

void MusicManager::PlayBGM()
{
	if (music_bgm) {
		music_bgm->Play();
	}
}

void MusicManager::StopBGM()
{
	if (music_bgm) {
		music_bgm->Stop();
	}
}
