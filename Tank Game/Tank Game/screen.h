#pragma once
#include "console.h"
#include "map.h"
#include "tank.h"

void ScreenInit();
void ScreenFlipping();
void ScreenClear();
void ScreenRelease();
void ScreenPrint(int x, int y, char* string);
void ScreenRemove(int x, int y, char* string);

void ScreenPrintFPS(int x, int y, double num);
void SetColor(unsigned short color);

void Render(int **map);
void RenderMap(const int **map);
void RenderLoading();
void Update();

double calculateFPS();

extern struct Tank g_Tank;