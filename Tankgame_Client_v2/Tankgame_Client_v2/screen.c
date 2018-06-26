#include "stdafx.h"
#include "screen.h"

HANDLE g_hScreen[3];
int g_nScreenIndex = 2;
int g_nScreenFlag = 0;
int g_nScreenFlag_Sub = 0;

int nFrames = 0;
double fps = 0;

extern int g_nPing;

int alignCenter(const char *str) {
	return (int)((SCREEN_SIZE_X / 2) - (strlen(str) / 2));
}


unsigned int WINAPI handleScreen(void *arg) {
	int **map = arg;
	char buffer[BUF_SIZE];
	static time_t nCurTime;
	int i, j;
	ExplosionPos exPos;
	int expLife[30][30] = { 0 };

	initializeScreen();

	nCurTime = clock();
	while (1) {
		switch (g_nScreenFlag) {
		case LOADING:
			while (g_nScreenFlag == LOADING) {
				static int state = 0;

				if (g_nScreenFlag_Sub == CONNECTION_FINISH) {
					g_nScreenFlag = MATCHING;
					ScreenClear();

					break;
				}
				Sleep(333);
				if (state)
					s_print(alignCenter("Loading ..."), SCREEN_SIZE_Y / 2, "Loading ...");
				else
					s_print(alignCenter("Loading ..."), SCREEN_SIZE_Y / 2, "Loading .. ");
				state = !state;

				switch (g_nScreenFlag_Sub) {
					case CONNECTION_FAIL:
						g_nScreenFlag_Sub = NONE;
						s_print(alignCenter("retry ..."), SCREEN_SIZE_Y / 2 + 1, "retry ...");
						break;
					case CONNECTION_SUCCESS:
						s_print(alignCenter("Connected"), SCREEN_SIZE_Y / 2 + 1, "Connected");
						sprintf(buffer, "Ping : %d", g_nPing);
						s_print(alignCenter(buffer), SCREEN_SIZE_Y / 2 + 2, buffer);
						Sleep(200);
						break;
					default:
						break;
				}
			}
			break;
		case MATCHING:

			s_print(alignCenter("Matching ..."), SCREEN_SIZE_Y / 2, "Matching ...");
			parseMap(&map, 1);
			while (g_nScreenFlag == MATCHING) {
				static time_t nElapseTime;
				static int nMinute, nSecond;

				if (g_nScreenFlag_Sub == MATCHING_FINISH) {
					g_nScreenFlag = INGAME;
					renderMap(map);
					g_nScreenIndex = 1;
					Sleep(3000);
					flipScreen();

					break;
				}

				nElapseTime = (clock() - nCurTime) / 1000;
				nMinute = (int)nElapseTime / 60;
				nSecond = nElapseTime % 60;
				sprintf(buffer, "%3d:%02d", nMinute, nSecond);
				s_print(alignCenter(buffer), SCREEN_SIZE_Y / 2 + 1, buffer);

				sprintf(buffer, "(%d / %d)", nCurUserCount, nMaxUserCount);
				s_print(alignCenter(buffer), SCREEN_SIZE_Y / 2 + 2, buffer);
			}
			break;
		case INGAME:

			renderFPSLabel();
			//renderPingLabel();
			while (g_nScreenFlag == INGAME) {
				int nBulletCount = nMaxUserCount * MAX_BULLET_COUNT;

				for (i = 0; i < nMaxUserCount; i++) {
					/*
					 * Render Tank... if different old position and current position
					 */
					if (g_Tank[i].oldX[g_nScreenIndex] != g_Tank[i].x ||
						g_Tank[i].oldY[g_nScreenIndex] != g_Tank[i].y) {
						sprintf(buffer, "  ");
						s_print(g_Tank[i].oldX[g_nScreenIndex]*2, g_Tank[i].oldY[g_nScreenIndex]+1, buffer);
						switch (g_Tank[i].nDirect) {
							case UP:
								sprintf(buffer, "▲"); break;
							case DOWN:
								sprintf(buffer, "▼"); break;
							case LEFT:
								sprintf(buffer, "◀"); break;
							case RIGHT:
								sprintf(buffer, "▶"); break;
						}
						if (g_Tank[i].id == nCID)
							SetColor(FOREGROUND_RED);
						else
							SetColor(FOREGROUND_RED | FOREGROUND_GREEN);


						EnterCriticalSection(&g_cs);
						s_print((int)g_Tank[i].x * 2, (int)g_Tank[i].y + 1, buffer);

						g_Tank[i].oldX[g_nScreenIndex] = g_Tank[i].x;
						g_Tank[i].oldY[g_nScreenIndex] = g_Tank[i].y;
						LeaveCriticalSection(&g_cs);

					}
				}
				SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

				for (j=0; j < nBulletCount; j++) {
					if (g_bullet[j].nDirect != DIE) {
						if (g_bullet[j].oldX[g_nScreenIndex] != g_bullet[j].x ||
							g_bullet[j].oldY[g_nScreenIndex] != g_bullet[j].y) {
							sprintf(buffer, "  ");
							s_print(g_bullet[j].oldX[g_nScreenIndex] * 2, g_bullet[j].oldY[g_nScreenIndex] + 1, buffer);
			
							switch (g_bullet[j].nDirect) {
								case UP:
								case DOWN:
									sprintf(buffer, "│"); break;
								case LEFT:
								case RIGHT:
									sprintf(buffer, "─"); break;
							}

							EnterCriticalSection(&g_cs);
							s_print((int)g_bullet[j].x * 2, (int)g_bullet[j].y + 1, buffer);
							g_bullet[j].oldX[g_nScreenIndex] = g_bullet[j].x;
							g_bullet[j].oldY[g_nScreenIndex] = g_bullet[j].y;
							LeaveCriticalSection(&g_cs);
						}

					}
				}

				while (!isEmptyR(&expQueue)) {
					deQueueR(&expQueue, &exPos);
					if (exPos.id < 0) {
						s_remove(exPos.x * 2, exPos.y + 1, 0, "※");
						s_remove(exPos.x * 2, exPos.y + 1, 1, "※");
						expLife[exPos.x][exPos.y] = (int)fps;
						continue;
					}

					sprintf(buffer, "※");
					s_print(exPos.x * 2, exPos.y + 1, buffer);
					s_remove(exPos.x * 2, exPos.y + 1, !g_nScreenIndex, "※");

					expLife[exPos.x][exPos.y] = (int)fps; // 약 1초간 유지될 수 있도록 (1초에 fps번 연산함)
				}
				for (i = 0; i < 30; i++) {
					for (j = 0; j < 30; j++) {
						if (--expLife[i][j] == 0) {
							s_remove(i * 2, j + 1, g_nScreenIndex, "  ");
							s_remove(i * 2, j + 1, !g_nScreenIndex, "  ");
						}
					}
				}

				calculateFPS();
				renderFPS(4, 0, fps);
				flipScreen();
			}
			break;
		case WINNER:
			ScreenClear();
			return 0;
			break;
		default:
			break;
		}
	}
	return 0;
}

void initializeScreen() {
	CONSOLE_CURSOR_INFO cci;
	COORD size = { SCREEN_SIZE_X, SCREEN_SIZE_Y };
	SMALL_RECT rect;
	rect.Left = 0;
	rect.Right = SCREEN_SIZE_X - 1;
	rect.Top = 0;
	rect.Bottom = SCREEN_SIZE_Y - 1;

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
#ifndef DEBUG
	SetConsoleActiveScreenBuffer(g_hScreen[g_nScreenIndex]);
#endif
}

void releaseScreen(int index) {
	CloseHandle(g_hScreen[index]);
}

void flipScreen() {
	Sleep(FLIP_DELAY);
#ifndef DEBUG
	SetConsoleActiveScreenBuffer(g_hScreen[g_nScreenIndex]);
#endif
	nFrames++;
	if (g_nScreenIndex == 0)
		g_nScreenIndex = 1;
	else
		g_nScreenIndex = 0;
}

/* ------------------------------------------------------------------------------------------
*	Colors
*		FOREGROUND_RED
*		FOREGROUND_BLUE
*		FOREGROUND_GREEN
*		FOREGROUND_INTENSITY
------------------------------------------------------------------------------------------ */
void SetColor(unsigned short color) {
	SetConsoleTextAttribute(g_hScreen[g_nScreenIndex], color);
}

void s_print(int x, int y, char *string) {
	DWORD dw;
	COORD CursorPosition = { x, y };
	SetConsoleCursorPosition(g_hScreen[g_nScreenIndex], CursorPosition);
	WriteFile(g_hScreen[g_nScreenIndex], string, strlen(string), &dw, NULL);
}

void s_puts(char *string) {
	DWORD dw;
	WriteFile(g_hScreen[g_nScreenIndex], string, strlen(string), &dw, NULL);
}

void ScreenClear() {
	COORD Coor = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(g_hScreen[g_nScreenIndex], ' ', 81 * SCREEN_SIZE_Y, Coor, &dw);
}

void renderLoading() {
	DWORD dw;
	COORD CursorPosition = { 25, 16 };
	unsigned char buffer[256] = {
		"          Loading ..." };
	SetConsoleActiveScreenBuffer(g_hScreen[2]);

	for (int i = 0; i < 10; i++) {
		SetConsoleCursorPosition(g_hScreen[2], CursorPosition);
		WriteFile(g_hScreen[2], buffer, strlen(buffer), &dw, NULL);
		CursorPosition.Y++;
	}
}

void renderFPSLabel() {
	DWORD dw;
	COORD coord = { 0, 0 };
	unsigned char buffer[10] = { 0 };
	sprintf(buffer, "  * fps :");
	SetConsoleCursorPosition(g_hScreen[0], coord);
	WriteFile(g_hScreen[0], buffer, sizeof(buffer), &dw, NULL);
	SetConsoleCursorPosition(g_hScreen[1], coord);
	WriteFile(g_hScreen[1], buffer, sizeof(buffer), &dw, NULL);

}

const char *getCharacter(int num);
void renderMap(const int **map) {
	DWORD dw;
	COORD coord = { 0, 1 };
	unsigned char buffer[BUF_SIZE] = { 0 };
	int i, j;

	for (j = 0; j < 30; j++) {
		for (i = 0; i < 30; i++) {
			strcat(buffer, getCharacter(map[j][i])); // map[j][i]에 담긴 상수에 해당하는 아스키 문자를 버퍼에 저장
		}
		SetConsoleCursorPosition(g_hScreen[0], coord);
		WriteFile(g_hScreen[0], buffer, sizeof(buffer), &dw, NULL);
		SetConsoleCursorPosition(g_hScreen[1], coord);
		WriteFile(g_hScreen[1], buffer, sizeof(buffer), &dw, NULL);
		coord.Y++;
		buffer[0] = '\0'; // 버퍼 초기화
	}
}

const char *getCharacter(int num) {
	switch (num) {
	case WALL:
		return CHARACTER_WALL;
	case BLANK: return  CHARACTER_BLANK;
	default:
		if (num > 0 && num <= BARRICADE) {
			return CHARACTER_BARRICADE;
		}
		exit(2426);
	}
}

/* =======================================================================================

Function calculateFPS ()
return double(calculated FPS)
ntime - timebase : 이전 계산으로부터 경과한 시간
== 500이면 (0.5s 경과 후)
nFrames (Flip된 횟수) / 0.5s
nFrames : 1(s) = x * nFrames : 500(ms)
then, x = 500

======================================================================================= */
double calculateFPS() {
	time_t ntime;
	static time_t timebase = 0;

	ntime = clock();
	if (ntime - timebase > 500) {
		fps = (double)(nFrames * 500) / (ntime - timebase);
		timebase = ntime;
		nFrames = 0;
	}
	// COUNTER STOP
	if (fps > 9999)
		return 9999;
	return fps;
}

void renderFPS(int x, int y, double num) {
	unsigned char buffer[16];

	sprintf(buffer, "%.2lf", num);
	s_print(x + 6, y, buffer);
}

void s_remove(int x, int y, int index, char* string) {
	DWORD dw;
	COORD CursorPosition = { x, y };
	SetConsoleCursorPosition(g_hScreen[index], CursorPosition);
	WriteFile(g_hScreen[index], string, strlen(string), &dw, NULL);
}

void renderPingLabel() {
	DWORD dw;
	COORD coord = { SCREEN_SIZE_X - 14, 0 };
	unsigned char buffer[9] = { 0 };
	sprintf(buffer, "* ping :");
	SetConsoleCursorPosition(g_hScreen[0], coord);
	WriteFile(g_hScreen[0], buffer, sizeof(buffer), &dw, NULL);
	SetConsoleCursorPosition(g_hScreen[1], coord);
	WriteFile(g_hScreen[1], buffer, sizeof(buffer), &dw, NULL);
}

void renderPing() {
	DWORD dw;
	COORD coord = { SCREEN_SIZE_X - 5, 0 };
	static int nOldPing = -1;
	unsigned char buffer[5];
	if (g_nPing != nOldPing) {
		sprintf(buffer, "%4d", g_nPing);
		SetConsoleCursorPosition(g_hScreen[0], coord);
		WriteFile(g_hScreen[0], buffer, sizeof(buffer), &dw, NULL);
		SetConsoleCursorPosition(g_hScreen[1], coord);
		WriteFile(g_hScreen[1], buffer, sizeof(buffer), &dw, NULL);
		nOldPing = g_nPing;
	}
}