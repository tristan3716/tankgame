#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include "screen.h"
#include "main.h"

int g_nScreenIndex=2;
int g_nOldScreenIndex;
HANDLE g_hScreen[3];
int nFrames = 0;
double fps = 0;
time_t ntime;
time_t timebase = 0; 

void ScreenInit(){
	CONSOLE_CURSOR_INFO cci; 
	COORD size = { 80, 32 };
	SMALL_RECT rect;
	rect.Left = 0;
	rect.Right = 80 - 1;
	rect.Top = 0;
	rect.Bottom = 32 - 1;

	g_hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0,
		NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleScreenBufferSize(g_hScreen[0], size);
	SetConsoleWindowInfo(g_hScreen[0], TRUE, &rect);

	g_hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0,
		NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleScreenBufferSize(g_hScreen[1], size);
	SetConsoleWindowInfo(g_hScreen[1], TRUE, &rect);

	g_hScreen[2] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0,
		NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleScreenBufferSize(g_hScreen[2], size);
	SetConsoleWindowInfo(g_hScreen[2], TRUE, &rect);

	cci.dwSize = 1;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(g_hScreen[0], &cci);
	SetConsoleCursorInfo(g_hScreen[1], &cci);
	SetConsoleCursorInfo(g_hScreen[2], &cci);
}

void ScreenFlipping(){
	//Sleep(22);
	SetConsoleActiveScreenBuffer(g_hScreen[g_nScreenIndex]);
	nFrames++;
	if (g_nScreenIndex == 0)
		g_nScreenIndex = 1;
	else if (g_nScreenIndex == 1)
		g_nScreenIndex = 0;
	else if (g_nScreenIndex == 2)
		g_nScreenIndex = 0;
}

void ScreenClear(){
	COORD Coor = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(g_hScreen[g_nScreenIndex], ' ', 80 * 32, Coor, &dw);
}

void ScreenRelease(){
	CloseHandle(g_hScreen[0]);
	CloseHandle(g_hScreen[1]);
	CloseHandle(g_hScreen[2]);
}

void ScreenPrint(int x, int y, char *string){
	DWORD dw;
	COORD CursorPosition = { x, y };
	SetConsoleCursorPosition(g_hScreen[g_nScreenIndex], CursorPosition);
	WriteFile(g_hScreen[g_nScreenIndex], string, strlen(string), &dw, NULL);
}

int getOldIndex(int n) {
	return !n;
}

void ScreenRemove(int x, int y, char* string) {
	DWORD dw;
	COORD CursorPosition = { x, y };
	SetConsoleCursorPosition(g_hScreen[!g_nScreenIndex], CursorPosition);
	WriteFile(g_hScreen[!g_nScreenIndex], string, strlen(string), &dw, NULL);
	//SetConsoleCursorPosition(g_hScreen[1], CursorPosition);
	//WriteFile(g_hScreen[1], string, strlen(string), &dw, NULL);
}

void ScreenPrintFPS(int x, int y, double num) {
	//COORD coord = { x + 8, y };
	unsigned char buffer[64];
	
	sprintf(buffer, "%.2lf", num);
	//SetConsoleCursorPosition(g_hScreen[g_nScreenIndex], coord);
	ScreenPrint(x+8, y, buffer);
}

void SetColor(unsigned short color){
	SetConsoleTextAttribute(g_hScreen[g_nScreenIndex], color);
}

const char *getCharacter(int num) {
	switch (num) {
		case MAP_WALL:
			return "■";
		case MAP_BLANK: return  "  ";
		default: 
			if (num > 0 && num <= MAP_BARRICADE) {
				return "▒";
			}
			exit(2426);
	}
}

void RenderLoading() {
	DWORD dw;
	COORD CursorPosition = { 25, 16 }; // Start from line 1
	unsigned char buffer[1][256] = { 
		"          Loading ..."};
	SetConsoleActiveScreenBuffer(g_hScreen[2]);

	for (int i = 0; i < 1; i++) {
		SetConsoleCursorPosition(g_hScreen[2], CursorPosition);
		WriteFile(g_hScreen[2], buffer[i], strlen(buffer[i]), &dw, NULL);
		CursorPosition.Y++;
	}
}

void RenderMap(const int **map) {
	DWORD dw;
	COORD CursorPosition = { 0, 0 };
	unsigned char buffer[1024] = { 0 };
	int i, j;

	//ScreenPrint(0, 0, "* fps : ");
	sprintf(buffer, "  * fps : ");
	SetConsoleCursorPosition(g_hScreen[0], CursorPosition);
	WriteFile(g_hScreen[0], buffer, sizeof(buffer), &dw, NULL);
	SetConsoleCursorPosition(g_hScreen[1], CursorPosition);
	WriteFile(g_hScreen[1], buffer, sizeof(buffer), &dw, NULL);
	CursorPosition.Y++;
	buffer[0] = '\0';

	for (j = 0; j < 30; j++) {
		for (i = 0; i < 30; i++) {
			strcat(buffer, getCharacter(map[j][i]));
		}
		SetConsoleCursorPosition(g_hScreen[0], CursorPosition);
		WriteFile(g_hScreen[0], buffer, sizeof(buffer), &dw, NULL);
		SetConsoleCursorPosition(g_hScreen[1], CursorPosition);
		WriteFile(g_hScreen[1], buffer, sizeof(buffer), &dw, NULL);
		CursorPosition.Y++;
		buffer[0] = '\0';
	}
}

void Update() {
	int i;
	clock_t nCurTime = clock();
	fps = calculateFPS();

	for (i = 0; i < 5; i++) {
		if (g_Tank_Bullet[i].nLife == 1) {
			if (nCurTime - g_Tank_Bullet[i].nOldMoveTime >= BULLET_MOVE_DELAY) {
				g_Tank_Bullet[i].state = FLAG_MOVED;
				g_Tank_Bullet[i].nOldPos = g_Tank_Bullet[i].nPos;
				switch (g_Tank_Bullet[i].nDirect) {
				case UP:
					g_Tank_Bullet[i].nPos.Y -= 1;
					break;
				case DOWN:
					g_Tank_Bullet[i].nPos.Y += 1;
					break;
				case LEFT:
					g_Tank_Bullet[i].nPos.X -= 2;
					break;
				case RIGHT:
					g_Tank_Bullet[i].nPos.X += 2;
					break;
				}
				g_Tank_Bullet[i].nOldMoveTime = nCurTime;
			}
		}
	}
}

void Render(int **map) {
	char str[100];
	int i;
	//ScreenClear();
	if (g_Tank.flag == FLAG_MOVED || g_Tank.flag == FLAG_HOLD) {
		ScreenRemove(g_Tank.nOldPos.X, g_Tank.nOldPos.Y, "  ");
	}
	for (i = 0; i < 5; i++) {
		if (g_Tank_Bullet[i].state == FLAG_MOVED || g_Tank_Bullet[i].state == FLAG_HOLD) {
			ScreenRemove(g_Tank_Bullet[i].nOldPos.X, g_Tank_Bullet[i].nOldPos.Y, "  ");
		}
	}

	//EnterCriticalSection(&g_cs);
	ScreenPrintFPS(0, 0, fps);
	if (g_Tank.flag == FLAG_MOVED || g_Tank.flag == FLAG_TURN || g_Tank.flag == FLAG_HOLD || g_Tank.flag == FLAG_DEFAULT) {
		switch (g_Tank.nDirect) {
			case UP:
				//SetColor(12);
				ScreenPrint(g_Tank.nPos.X, g_Tank.nPos.Y, "▲"); break;
			case DOWN:
				//SetColor(12);
				ScreenPrint(g_Tank.nPos.X, g_Tank.nPos.Y, "▼"); break;
			case RIGHT:
				//SetColor(12);
				ScreenPrint(g_Tank.nPos.X, g_Tank.nPos.Y, "▶"); break;
			case LEFT:
				//SetColor(12);
				ScreenPrint(g_Tank.nPos.X, g_Tank.nPos.Y, "◀"); break;
			default:break;
		}
		switch(g_Tank.flag){
			case FLAG_DEFAULT:
			case FLAG_MOVED:
			case FLAG_TURN:
				g_Tank.flag = FLAG_HOLD; break;
			case FLAG_HOLD:
				g_Tank.flag = FLAG_NONE; break;
		}
	}
	//if (isMoved(g_Tank.flag))
		//ScreenRemove(g_Tank.nOldPos.X, g_Tank.nOldPos.Y, "  ");

	// DEBUG SOURCE
	//SetColor(1);
	sprintf(str, "CS : %d", g_nScreenIndex);
	ScreenPrint(62, 0, str);
	sprintf(str, "c : (%d, %d)", g_Tank.nPos.X, g_Tank.nPos.Y);
	ScreenPrint(62, 1, str); 
	sprintf(str, "old : (%d, %d)", g_Tank.nOldPos.X, g_Tank.nOldPos.Y);
	ScreenPrint(62, 2, str);


	sprintf(str, "g_Tank");
	ScreenPrint(62, 20, str);
	sprintf(str, "* nDirect : %d", g_Tank.nDirect);
	ScreenPrint(62, 21, str);

	// Bullet Print
	for (i = 0; i < 5; i++) {
		if (g_Tank_Bullet[i].state == FLAG_MOVED || g_Tank_Bullet[i].state == FLAG_TURN || g_Tank_Bullet[i].state == FLAG_HOLD || g_Tank_Bullet[i].state == FLAG_DEFAULT) {
			if (g_Tank_Bullet[i].nLife == 1) {
				if (g_Tank_Bullet[i].nDirect == UP ||
					g_Tank_Bullet[i].nDirect == DOWN)
					ScreenPrint(g_Tank_Bullet[i].nPos.X, g_Tank_Bullet[i].nPos.Y, "│");
				else
					ScreenPrint(g_Tank_Bullet[i].nPos.X, g_Tank_Bullet[i].nPos.Y, "─");
			}
			switch (g_Tank_Bullet[i].state) {
			case FLAG_DEFAULT:
			case FLAG_MOVED:
			case FLAG_TURN:
				g_Tank_Bullet[i].state = FLAG_HOLD; break;
			case FLAG_HOLD:
				g_Tank_Bullet[i].state = FLAG_NONE; break;
			}
		}
	}
	sprintf(str, "nFT :%d, State :%d", g_Tank.nFireTime, hBullet);
	ScreenPrint(62, 3, str);
	for (i = 0; i < 5; i++) {
		sprintf(str, "%d : %d, %d", g_Tank_Bullet[i].nLife, 
			g_Tank_Bullet[i].nPos.X, g_Tank_Bullet[i].nPos.Y);
		ScreenPrint(62, 4+i, str);
	}

	//LeaveCriticalSection(&g_cs);

	ScreenFlipping();
}

double calculateFPS() {
	ntime = clock();
	if (ntime - timebase > 500){
		fps = (double)(nFrames*500) / (ntime - timebase);
		timebase = ntime;
		nFrames = 0;
	}
	if (fps > 9999) return 9999;
	return fps;
}