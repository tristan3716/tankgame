#pragma once

#include "main.h"

typedef enum _TILESET {WALL=2001, BARRICADE=1000, BLANK=0} TILESET;
#define CHARACTER_WALL "бс"
#define CHARACTER_BARRICADE "в╞"
#define CHARACTER_BLANK "  "

void parseMap(int ***map, int stage_number);
const char *getMapPath(const int stage_number);