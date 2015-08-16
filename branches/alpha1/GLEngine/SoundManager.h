#pragma once
#include <stdio.h>

class SoundManager {
public:
	static const char* SOUND_GAMEOVER;
	static const char* SOUND_GAMEWIN;
	static const char* SOUND_JUMP;

	static void play(const char* id);
};