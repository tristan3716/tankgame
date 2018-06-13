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
	printf("HELLO");

	renderLoading();
	printf("HELLO");
	//Sleep(500);
	flipScreen();

	printf("HELLO");
	renderFPSLabel();
	printf("HELLO");
	renderMap(arg);
	printf("HELLO");
	//ScreenFlipping();
	while (1) {
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
	struct Tank tank[5];
	int i;

	SetConsole();
	for (i = 0; i < 5; i++) {
		initiateTank(&tank[i]);
	}
	initiateSoundList(&sHd);

	InitializeCriticalSection(&g_cs);


	parseMap(&map, 1);
	//print(map, 30, 30);

	// warning C4028: ���� �Ű� ���� 1��(��) ����� �ٸ��ϴ�. ????
	hThread = (HANDLE)_beginthreadex(NULL, 0, handleKey, NULL, 0, (unsigned*)&dwThreadID);
	// Screen Handle
	hThread2 = (HANDLE)_beginthreadex(NULL, 0, handleScreen, map, 0, (unsigned*)&dwThreadID);


	//setSoundParameters(&sparams, &sHd, SOUND_EXPLOSION);
	//hThread = (HANDLE)_beginthreadex(NULL, 0, playSoundThread, &sparams, 0, (unsigned*)&dwThreadID);

	WaitForSingleObject(hThread, INFINITE);
	WaitForSingleObject(hThread2, INFINITE);

	ScreenRelease();
	DeleteCriticalSection(&g_cs);

	return 0;
}