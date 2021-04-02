#pragma once

#include <Windows.h>
#include <cstdint>
#include "ftd2xx.h"

#define PIXEL_SCALE 5

extern unsigned char* frameBuffer;

extern HANDLE com;
extern FT_HANDLE ftCom;

extern bool running;

typedef struct {
	int x;
	int y;
} VERTEX;

bool initDisplay(char* port);

void runDisplayLoop();

//function declarations mostly for testing purposes
void clearScreen();

void drawCharacter(char c);

void drawCircle(int xc, int yc, int radius, uint16_t color, bool filled);

bool drawLine(VERTEX v1, VERTEX v2, uint16_t color);

void drawRectangle(int sx, int sy, int width, int height, uint16_t color, bool filled);