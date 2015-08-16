#include "SoundManager.h"

const char* SoundManager::SOUND_GAMEOVER = "Gameover.mp3";

const char* SoundManager::SOUND_GAMEWIN = "Gamewin.mp3";

const char* SoundManager::SOUND_JUMP = "jump.mp3";

#ifdef WIN32
void SoundManager::play(const char* id) {
	printf("\nPlaying : %s", id);
}
#elif ANDROID
void SoundManager::play(const char* id) {
	//call java method
	//printf("\nAndroid is playing: %s", id);
}
#endif