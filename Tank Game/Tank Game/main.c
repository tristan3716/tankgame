#include "main.h"


unsigned int WINAPI playSoundThread(void *arg) {
	struct Params *params = (struct Params *)arg;
	params->sHd->playSound(params->sHd, 
		params->sHd->getSoundPath(params->sHd, params->id));
	return 0;
}

unsigned int WINAPI handleScreen(void *arg) {
	//console_setWhite();
	ScreenInit();

	renderLoading();
	//Sleep(500);
	flipScreen();

	renderFPSLabel();
	renderMap(arg);
	//ScreenFlipping();
	while (TRUE) {
		Update();
		Render(arg);
	}
	return 0;
}

int main(void) {
	struct SoundList sHd;
	struct Params sparams;
	HANDLE hThread = NULL;
	HANDLE hThread2 = NULL;
	DWORD dwThreadID = (DWORD)NULL;
	int **map;
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr;
	int clntAdrSize;

	SetConsole();
	initiateTank();
	initiateSoundList(&sHd);

	InitializeCriticalSection(&g_cs);


	parseMap(&map, 1);
	//print(map, 30, 30);

	// Screen Handle
	hThread = (HANDLE)_beginthreadex(NULL, 0, handleScreen, map, 0, (unsigned*)&dwThreadID);

	// warning C4028: 정식 매개 변수 1이(가) 선언과 다릅니다. ????
	hThread = (HANDLE)_beginthreadex(NULL, 0, handleKey, NULL, 0, (unsigned*)&dwThreadID);

	//setSoundParameters(&sparams, &sHd, SOUND_EXPLOSION);
	//hThread = (HANDLE)_beginthreadex(NULL, 0, playSoundThread, &sparams, 0, (unsigned*)&dwThreadID);
	
	WaitForSingleObject(hThread, INFINITE);

	ScreenRelease();
	DeleteCriticalSection(&g_cs);

	return 0;
}