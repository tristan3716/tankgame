#ifndef _SOUND_H
#define _SOUND_H
#pragma comment(lib,"winmm.lib")

#include "main.h"

#define SOUND_EXPLOSION 101

typedef struct Params Params;

struct SoundList {
	int count;

	void(*stopSound)();
	void(*playSound)(const struct SoundList *this, const char *path);
	char *(*getSoundPath)(const struct SoundList *this, int index);
};

void initializeSound(struct SoundList *this);

#endif _SOUND_H