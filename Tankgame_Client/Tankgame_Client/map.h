#pragma once

#include "main.h"

#define TANK_DN 101 // ¡ã
#define TANK_DS 102 // ¡å
#define TANK_DW 103 // ¢¸
#define TANK_DE 104 // ¢º

#define BULLET 111 // ¡¤

#define MAP_SIZE_WIDTH 30
#define MAP_SIZE_HEIGHT 30

typedef enum _TILESET {WALL=2001, BARRICADE=1000, BLANK=0} TILESET;
#define CHARACTER_WALL "¡á"
#define CHARACTER_BARRICADE "¢Æ"
#define CHARACTER_BLANK "  "

void parseMap(int ***map, int stage_number);
const char *getMapPath(const int stage_number);