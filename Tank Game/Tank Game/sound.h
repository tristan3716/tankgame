#ifndef SOUND_H
#define SOUND_H
#pragma comment(lib,"winmm.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strcpy
#include <Windows.h> // mbstowcs
#include "main.h"

#define SOUND_EXPLOSION 101

typedef struct Params Params;

struct SoundList {
	int count;

	void (*stopSound)();
	void (*playSound)(const struct SoundList *this, const char *path);
	char *(*getSoundPath)(const struct SoundList *this, int index);
} ;

void initiateSoundList(struct SoundList *this);
void setSoundParameters(struct Params *params, struct SoundList *sHd, int id);

#endif SOUND_H