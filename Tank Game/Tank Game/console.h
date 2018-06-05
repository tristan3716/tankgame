
#pragma once

#define col GetStdHandle(STD_OUTPUT_HANDLE)
#define console_setWhite() SetConsoleTextAttribute (col, 0x000f)
#define console_setRed() SetConsoleTextAttribute (col, 0x000c)
#define console_setYellow() SetConsoleTextAttribute (col, 0x000e)
#define console_setGray() SetConsoleTextAttribute (col, FOREGROUND_RED)

void moveCursur(int x, int y);
void SetConsole();
