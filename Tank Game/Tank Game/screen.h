#pragma once
#include "console.h"
#include "map.h"
#include "tank.h"

void ScreenInit();
void flipScreen();
//void ScreenClear();
void ScreenRelease();
void ScreenPrint(int x, int y, char* string);
void ScreenRemove(int x, int y, char* string);

void renderFPS(int x, int y, double num);
void SetColor(unsigned short color);

void Render(int **map);

void renderLoading();
void renderFPSLabel();
void renderMap(const int **map);

void Update();

double calculateFPS();

extern struct Tank g_Tank;