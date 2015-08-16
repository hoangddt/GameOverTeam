#pragma once

#define SOUND_GAMEOVER		"gameover.mp3"
#define SOUND_GAMEWIN		"gamewin.mp3"
#define SOUND_GAMEWIN_STAGE "gamewinstage.mp3"
#define SOUND_LEVELUP		"levelup.mp3"
#define SOUND_JUMPSTART		"jumpstart.mp3"
#define SOUND_JUMPEND		"jumpend.mp3"
#define SOUND_BG_STAGE1		"bgstage1.mp3"
#define SOUND_BG_STAGE2		"bgstage2.mp3"

class SoundManager {
private:
	static SoundManager* mInstance;
	const char* mRootPath;
	bool mIsMute;

	SoundManager();
	SoundManager(const SoundManager &){} 
	SoundManager& operator =(const SoundManager &){}
	~SoundManager(void);
public:
	static SoundManager* getInstance();
	static void destroyInstance();

	void init(const char* soundFolder);

	void setMute(bool mute);

	void play(const char* id, bool repeat = false);

	void stop(const char* id);
};