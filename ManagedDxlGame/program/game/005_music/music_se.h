#pragma once

class MusicSE {
public:
	explicit MusicSE(const std::string& path);
	~MusicSE();

	void Play();
	void Stop();

private:
	int music_se_hdl = 0;
};