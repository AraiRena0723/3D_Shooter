#include "../../dxlib_ext/dxlib_ext.h"
#include "music_se.h"

MusicSE::MusicSE(const std::string& path)
{
	music_se_hdl = LoadSoundMem(path.c_str());
}

MusicSE::~MusicSE()
{
	DeleteSoundMem(music_se_hdl);
}

void MusicSE::Play()
{
	PlaySoundMem(music_se_hdl, DX_PLAYTYPE_BACK);
}

void MusicSE::Stop()
{
	StopSoundMem(music_se_hdl);
}
