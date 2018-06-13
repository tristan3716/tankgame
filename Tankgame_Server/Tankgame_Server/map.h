#ifndef _MAP_H
#define _MAP_H

//#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define CHARACTER_WALL "бс"
#define CHARACTER_BARRICADE "в╞"
#define CHARACTER_BLANK "  "

typedef enum _TILESET { WALL = 2001, BARRICADE = 1000, BLANK = 0 } TILESET;

void parseMap(int ***map, int stageNum);
const char *getMapPath(const int stage_number);

#endif _MAP_H