#pragma once

#include <windows.h>

void gotoxy(int x, int y);
void ShowConsoleCursor(bool showFlag);
void setColor(int color);

void setColor(const std::string& color);
