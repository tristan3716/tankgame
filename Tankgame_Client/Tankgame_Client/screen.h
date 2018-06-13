#ifndef _SCREEN_H
#define _SCREEN_H

#include <windows.h>
#include <time.h>
#include <stdio.h>
#include "main.h"
#include "map.h"

#define SCREEN_SIZE_X 61
#define SCREEN_SIZE_Y 32

typedef enum _SCREEN_FLAG {
	NONE=3000, 
	LOADING=4100, CONNECTION_FAIL=4101, CONNECTION_SUCCESS=4102, CONNECTION_FINISH=4103,
	MATCHING=4200, MATCHING_FINISH=4201,
	INGAME=4300, RENDER_TANK=4301
} SCREEN_FLAG;

unsigned int WINAPI handleScreen(void *arg);

void initializeScreen();
void releaseScreen(int index);

void flipScreen();
void ScreenClear();

void s_print(int x, int y, char *string);
void s_puts(char *string);

void ScreenRemove(int x, int y, char* string);

void SetColor(unsigned short color);

void Render(int **map);

void renderLoading();
void renderFPSLabel();
void renderPingLabel();
void renderFPS(int x, int y, double num);
void renderPing();
void renderMap(const int **map);

void Update();

double calculateFPS();


extern int nMaxUserCount;
extern int nCurUserCount;
extern int nCID;
extern int nMapNumber;

#endif _SCREEN_H