#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include "screen.h"
#include "main.h"

HANDLE g_hScreen[3];
int g_nScreenIndex=2;
//int g_nOldScreenIndex;

int nFrames = 0;
double fps = 0;

/* =======================================================================================

	Function ScreenInit ()
		Screen()
	Function ScreenRelease ()
		~Screen()

======================================================================================= */
void ScreenInit() {
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

void ScreenRelease() {
	CloseHandle(g_hScreen[0]);
	CloseHandle(g_hScreen[1]);
	CloseHandle(g_hScreen[2]);
}

/* =======================================================================================

	Function flipScreen ()
		SetActiveBuffer (g_nScreenIndex)
		g_nScreenIndex = !g_nScreenIndex

======================================================================================= */
void flipScreen(){
	//Sleep(990);
	SetConsoleActiveScreenBuffer(g_hScreen[g_nScreenIndex]);
	nFrames++;
	if (g_nScreenIndex == 0)
		g_nScreenIndex = 1;
	else
		g_nScreenIndex = 0;
}

void SetColor(unsigned short color) {
	SetConsoleTextAttribute(g_hScreen[g_nScreenIndex], color);
}

void ScreenPrint(int x, int y, char *string){
	DWORD dw;
	COORD CursorPosition = { x, y };
	SetConsoleCursorPosition(g_hScreen[g_nScreenIndex], CursorPosition);
	WriteFile(g_hScreen[g_nScreenIndex], string, strlen(string), &dw, NULL);
}
/* 미사용
int getOldIndex(int n) {
	return !n;
}
*/

/* 성능을 이유로 미사용
void ScreenClear() {
	COORD Coor = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(g_hScreen[g_nScreenIndex], ' ', 80 * 32, Coor, &dw);
}
*/

/* =======================================================================================

	Function renderLoading ()
		ScreenBuffer1이 작성 완료될때까지 출력하는 Buffer3 (로딩화면)

	** 이후에 지우지 않고 재사용할 내용 작성
	   Buffer1, 2에 모두 작성함

	Function renderFPSLabel ()
		* fps :
		라벨 출력

	Function renderMap (const map)
		ScreenBuffer1, 2에 모두 Map을 렌더링함
	Function getCharacter (num)
		return char[2]
		map[x][y]에서 값에 해당하는 2바이트 아스키 문자를 반환함

======================================================================================= */
void renderLoading() {
	DWORD dw;
	COORD CursorPosition = { 25, 16 };
	unsigned char buffer[1][256] = { 
		"          Loading ..."};
	SetConsoleActiveScreenBuffer(g_hScreen[2]);

	for (int i = 0; i < 1; i++) {
		SetConsoleCursorPosition(g_hScreen[2], CursorPosition);
		WriteFile(g_hScreen[2], buffer[i], strlen(buffer[i]), &dw, NULL);
		CursorPosition.Y++;
	}
}

void renderFPSLabel() {
	DWORD dw;
	COORD coord = { 0, 0 };
	unsigned char buffer[16] = { 0 };
	sprintf(buffer, "  * fps : ");
	SetConsoleCursorPosition(g_hScreen[0], coord);
	WriteFile(g_hScreen[0], buffer, sizeof(buffer), &dw, NULL);
	SetConsoleCursorPosition(g_hScreen[1], coord);
	WriteFile(g_hScreen[1], buffer, sizeof(buffer), &dw, NULL);
	
}

const char *getCharacter(int num);
void renderMap(const int **map) {
	DWORD dw;
	COORD coord = { 0, 1 };
	unsigned char buffer[1024] = { 0 };
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
	case MAP_WALL:
		return "■";
	case MAP_BLANK: return  "  ";
	default:
		if (num > 0 && num <= MAP_BARRICADE) {
			return "▨";
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

/* =======================================================================================

	Function Update ()
		프로그램 수행 상의 값 변경이 실제로 이루어지는 부분
		calculateFps
		calculateBulletPosition

======================================================================================= */
void Update() {
	int i;
	clock_t nCurTime;
	fps = calculateFPS();

	/* ---------------------------------------------------------------------------------------
		Bullet Position Calculator
			Bullet[i]의 이전 이동 시각과 현재 시각의 차이가 이동딜레이보다 크면
			FLAG = MOVED, Old Position 설정 후 실제 이동 처리
	--------------------------------------------------------------------------------------- */
	nCurTime = clock();
	for (i = 0; i < 5; i++) {
		if (g_Tank_Bullet[i].nLife == 1) {
			if (nCurTime - g_Tank_Bullet[i].nOldMoveTime >= BULLET_MOVE_DELAY) {
				g_Tank_Bullet[i].state = MOVED;
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

/* =======================================================================================

	Function Render (map)
		Flow
		cls -> Render -> Flip
		변화하는 부분 cls
		변화하는 부분 Render
		Flip (!g_nScreenIndex)

   ======================================================================================= */
void renderFPS(int x, int y, double num) {
	unsigned char buffer[16];

	sprintf(buffer, "%.2lf", num);
	ScreenPrint(x + 6, y, buffer);
}

void ScreenRemove(int x, int y, char* string) {
	DWORD dw;
	COORD CursorPosition = { x, y };
	SetConsoleCursorPosition(g_hScreen[g_nScreenIndex], CursorPosition);
	WriteFile(g_hScreen[g_nScreenIndex], string, strlen(string), &dw, NULL);
	SetConsoleCursorPosition(g_hScreen[!g_nScreenIndex], CursorPosition);
	WriteFile(g_hScreen[!g_nScreenIndex], string, strlen(string), &dw, NULL);
}

//#define __DEBUG__
void Render(int **map) {
	char buffer[64]; // DEBUG Message 출력을 위한 sprintf buffer
	int i; // index

	/* 렌더링 성능 향상을 위해 미사용, 변화하는 부분만 재출력 */
	// ScreenClear(); /* = system("cls"); */


	/* ---------------------------------------------------------------------------------------
		Screen Remove
			FLAG : 하단 Tank Print 참조
			Tank 및 Bullet의 이전 위치 Buffer1, 2를 모두 지워줌
			※ 주의할 점
				- Screen Remove 과정에서 새로 출력할 경우 동기화 오류 생길 가능성
	   --------------------------------------------------------------------------------------- */
	if (g_Tank.nFlag == MOVED || g_Tank.nFlag == HOLD) {
		ScreenRemove(g_Tank.nOldPos.X, g_Tank.nOldPos.Y, "  ");
	}
	for (i = 0; i < 5; i++) {
		if (g_Tank_Bullet[i].state == MOVED || g_Tank_Bullet[i].state == HOLD) {
			ScreenRemove(g_Tank_Bullet[i].nOldPos.X, g_Tank_Bullet[i].nOldPos.Y, "  ");
		}
	}

	//EnterCriticalSection(&g_cs);
	/* ---------------------------------------------------------------------------------------
		FPS Print
			Update에서 calculateFPS 함수를 통해 계산된 값을 출력
			COORD:(8, 0) - 값 부분만 변화하면서 출력 ("* fps "는 출력 후 지우지 않고 재사용)
	   --------------------------------------------------------------------------------------- */
	renderFPS(2, 0, fps);

	/* ---------------------------------------------------------------------------------------
		Tank Print
			connected Client의 수 만큼 반복하여 출력, 색 구분 필요

			** FLAG **
			DEFAULT : Initialized, 시작 지점에 Print 후 HOLD 상태로 변화
			MOVED	: 이동한 상태 -> 현재 위치 Print 및 이전 위치 Remove -> HOLD 상태로 변화
			TURN	: 회전한 상태(제자리에서) -> 현재 위치 Print 
					  (Remove X : 위치 변화 없으므로 현재 위치 출력과 동시에 지워짐)
						-> HOLD 상태로 변화
			HOLD	: 대기 상태 -> 2개의 버퍼 모두에 Print 하기 위해서 1프레임 대기 ( Print 수행)
			NONE	: 아무 동작도 하지 않음
	   --------------------------------------------------------------------------------------- */
	
	if (g_Tank.nFlag == MOVED || g_Tank.nFlag == TURN || g_Tank.nFlag == HOLD || g_Tank.nFlag == DEFAULT) {
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
		}
		switch(g_Tank.nFlag){
			case DEFAULT:
			case MOVED:
			case TURN:
				g_Tank.nFlag = HOLD; break;
			case HOLD:
				g_Tank.nFlag = NONE; break;
		}
	}
	//if (isMoved(g_Tank.flag))
		//ScreenRemove(g_Tank.nOldPos.X, g_Tank.nOldPos.Y, "  ");

	// DEBUG SOURCE
	//SetColor(1);
	//Sleep(500);
#ifdef __DEBUG__
	sprintf(buffer, "CS : %d", g_nScreenIndex);
	ScreenPrint(62, 0, buffer);
	sprintf(buffer, "c : (%d, %d)", g_Tank.nPos.X, g_Tank.nPos.Y);
	ScreenPrint(62, 1, buffer);
	sprintf(buffer, "old : (%d, %d)", g_Tank.nOldPos.X, g_Tank.nOldPos.Y);
	ScreenPrint(62, 2, buffer);
	sprintf(buffer, "state : %d", g_Tank.flag);
	ScreenPrint(62, 12, buffer);


	sprintf(buffer, "g_Tank");
	ScreenPrint(62, 20, buffer);
	sprintf(buffer, "* nDirect : %d", g_Tank.nDirect);
	ScreenPrint(62, 21, buffer);
#endif // __DEBUG__

	/* ---------------------------------------------------------------------------------------
		Bullet Print
		connected Client * nLeftBulletCount 만큼 반복, 색 구분 X
	   --------------------------------------------------------------------------------------- */
	for (i = 0; i < 5; i++) {
		if (g_Tank_Bullet[i].state == MOVED || g_Tank_Bullet[i].state == TURN
			|| g_Tank_Bullet[i].state == HOLD || g_Tank_Bullet[i].state == DEFAULT) {
			if (g_Tank_Bullet[i].nLife == 1) {
				if (g_Tank_Bullet[i].nDirect == UP ||
					g_Tank_Bullet[i].nDirect == DOWN)
					ScreenPrint(g_Tank_Bullet[i].nPos.X, g_Tank_Bullet[i].nPos.Y, "│");
				else
					ScreenPrint(g_Tank_Bullet[i].nPos.X, g_Tank_Bullet[i].nPos.Y, "─");
			}
			switch (g_Tank_Bullet[i].state) {
			case DEFAULT:
			case MOVED:
			case TURN:
				g_Tank_Bullet[i].state = HOLD; break;
			case HOLD:
				g_Tank_Bullet[i].state = NONE; break;
			}
		}
	}
#ifdef __DEBUG__
	//sprintf(buffer, "nFT :%d, State :%d", FIRE_DELAY, hBullet);
	ScreenPrint(62, 3, buffer);
	for (i = 0; i < 5; i++) {
		sprintf(buffer, "%d : %d, %d", g_Tank_Bullet[i].nLife,
			g_Tank_Bullet[i].nPos.X, g_Tank_Bullet[i].nPos.Y);
		ScreenPrint(62, 4+i, buffer);
	}
#endif __DEBUG__

	//LeaveCriticalSection(&g_cs);

	flipScreen();
}
