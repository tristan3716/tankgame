#pragma once

#include "main.h"

#define TANK_DN 101 // ��
#define TANK_DS 102 // ��
#define TANK_DW 103 // ��
#define TANK_DE 104 // ��

#define BULLET 111 // ��

#define MAP_WALL 2001 // ��
#define MAP_BARRICADE 1000 // �� 1~1000 : Barricade (endurability)
#define MAP_BLANK 0

#define MAP_SIZE_WIDTH 30
#define MAP_SIZE_HEIGHT 30

void initiateMap(int ***map);
void parseMap(int ***map, int stage_number);
const char *getMapPath(const int stage_number);
void temp_drawMap(int ***map);