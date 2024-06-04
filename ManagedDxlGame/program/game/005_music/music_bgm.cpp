#include "../../dxlib_ext/dxlib_ext.h"
#include "music_bgm.h"

MusicBGM::MusicBGM(const std::string& path)
{
	music_bgm_hdl = LoadSoundMem(path.c_str());
}

MusicBGM::~MusicBGM()
{
	DeleteSoundMem(music_bgm_hdl);
}

void MusicBGM::Play()
{
	PlaySoundMem(music_bgm_hdl, DX_PLAYTYPE_LOOP);
}

void MusicBGM::Stop()
{
	StopSoundMem(music_bgm_hdl);
}
