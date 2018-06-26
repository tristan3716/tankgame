#include "stdafx.h"
#include "sound.h"

void SoundList_stopSound() {
	PlaySound(NULL, 0, 0);
}

void SoundList_playSound(const struct SoundList *this, const char *path) {
	int length = (int)strlen(path) + 1;
	wchar_t *lpath = (wchar_t*)malloc(sizeof(wchar_t) * length);
	mbstowcs(lpath, path, length);
	PlaySound(lpath, NULL, SND_FILENAME | SND_NODEFAULT);
}

char *SoundList_getSoundPath(const struct SoundList *this, int index) {
	switch (index) {
	case SOUND_EXPLOSION:
		return "grenade.wav";
	default:
		exit(2424);
	}
}

void initializeSound(struct SoundList *this) {
	this->stopSound = SoundList_stopSound;
	this->playSound = SoundList_playSound;
	this->getSoundPath = SoundList_getSoundPath;
}