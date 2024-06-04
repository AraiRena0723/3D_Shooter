#pragma once

class MusicBGM {
public:
	explicit MusicBGM(const std::string& path);
	~MusicBGM();

	void Play();
	void Stop();
private:
	int music_bgm_hdl = 0;
};