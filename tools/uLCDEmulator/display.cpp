#include "display.h"

#include <cstdint>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "Resources/STBImage/STBImage.h"

FT_HANDLE ftCom = NULL;
HANDLE com = NULL;
uint16_t backgroundColor = 0;
uint16_t textXMultiplier = 1;
uint16_t textYMultiplier = 1;
int16_t textXPos = 0;
int16_t textYPos = 0;
uint16_t textForeground = 0xFFFF;
uint16_t textBackground = 0;
int16_t originX = 0;
int16_t originY = 0;
int16_t textXSpacing = 0;
int16_t textYSpacing = 0;
uint16_t outlineColor = 0;
bool	 textItalic = false;
bool	 textBold = false;
bool	 textInvertColors = false;
bool	 textUnderline = false;
bool	 penSizeFilled = false;
unsigned char* fontImage;
bool	timedout = false;
bool	running = true;

bool loadSettings() {
	HANDLE hSettings = CreateFileA("Assets/settings.yml", FILE_READ_ACCESS, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hSettings == NULL) {
		std::cout << "Failed to load settings from Assets/settings.yml " << GetLastError() << std::endl;
		return false;
	}

	char buf[128];
	ZeroMemory(buf, 128);
	DWORD size = 0;

	ReadFile(hSettings, buf, 128, &size, NULL);

	if (size == 0) {
		std::cout << "Failed to load settings from Assets/settings.yml " << GetLastError() << std::endl;
		return false;
	}

	//gah, I'll do this later...
}

bool initDisplayCOM(char* port) {
	char buffer[40];
	ZeroMemory(buffer, 40);
	strcat_s(buffer, 40, "\\\\.\\");
	strcat_s(buffer, 40, port);
	com = CreateFileA(buffer, FILE_READ_ACCESS | FILE_WRITE_ACCESS, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (!com) {
		std::cout << "Failed to open the COM port: " << port << std::endl;
		return false;
	}

	DCB comParams;

	BOOL ok = GetCommState(com, &comParams);

	if (!ok) {
		std::cout << "Failed to open com port configuration: " << GetLastError() << std::endl;
		return false;
	}

	comParams.BaudRate = 115200;
	comParams.StopBits = 1;
	comParams.Parity = PARITY_NONE;
	comParams.ByteSize = 8;

	ok = SetCommState(com, &comParams);

	if (!ok) {
		std::cout << "Failed to set com port configuration: " << GetLastError() << std::endl;
		return false;
	}

	COMMTIMEOUTS timeouts;

	ok = GetCommTimeouts(com, &timeouts);

	if (!ok) {
		std::cout << "Failed to open com port timeouts: " << GetLastError() << std::endl;
		return false;
	}

	timeouts.ReadIntervalTimeout = 3000;
	timeouts.ReadTotalTimeoutMultiplier = 3000;

	ok = SetCommTimeouts(com, &timeouts);

	if (!ok) {
		std::cout << "Failed to set com port timeouts: " << GetLastError() << std::endl;
		return false;
	}
}

bool initDisplayFT() {
	FT_STATUS ftStatus; 
	DWORD numDevs;

	char* BufPtrs[3];
	// pointer to array of 3 pointers
	char Buffer1[64];
	// buffer for description of first device
	char Buffer2[64];
	// buffer for description of second device
	// initialize the array of pointers
	BufPtrs[0] = Buffer1;
	BufPtrs[1] = Buffer2;
	BufPtrs[2] = NULL;// last entry should be NULL
	
	ftStatus = FT_ListDevices(BufPtrs,&numDevs,FT_LIST_ALL|FT_OPEN_BY_DESCRIPTION); 
	
	if (ftStatus == FT_OK) {
		// FT_ListDevices OK, product descriptions are in Buffer1 and Buffer2, and 
		
		if (numDevs == 0) {
			//No connected device
			std::cout << "No device connected.";
			return false;
		}
	}
	else {
		std::cout << "Failed to list FTDI devices.";
		return false;
	}

	ftStatus = FT_Open(0, &ftCom);

	if (ftStatus != FT_OK) {
		//Failed to open device

		std::cout << "Failed to open FT device: " << Buffer1 << std::endl;
		return false;
	}

	std::cout << "Opened FT device: " << Buffer1 << std::endl;

	ftStatus = FT_SetBaudRate(ftCom, 1500000);

	ftStatus |= FT_SetDataCharacteristics(ftCom, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE);

	if (ftStatus != FT_OK) {
		//Failed to configure device

		std::cout << "Failed to configure FT device: " << Buffer1 << std::endl;
		return false;
	}

	ftStatus = FT_SetTimeouts(ftCom, 3000, 3000);

	if (ftStatus != FT_OK) {
		//Failed to configure device

		std::cout << "Failed to configure timeouts for FT device: " << Buffer1 << std::endl;
		return false;
	}

	return true;
}

bool initDisplay(char* port) {
	if (strcmp(port, "D2XX") == 0) {
		//using FTDI driver
		if (!initDisplayFT()) {
			return false;
		}
	}
	else {
		if (!initDisplayCOM(port)) {
			return false;
		}
	}

	std::cout << "Successfully opened the port, starting service.\n";

	fontImage = nullptr;
	int width, height, bpp;
	fontImage = stbi_load("Assets/font.png", &width, &height, &bpp, 1);

	if (fontImage == nullptr) {
		std::cout << "Failed to open font image at Assets/font.png.\n";
		return false;
	}

	if (width != 128 || height != 128) {
		std::cout << "Invalid font image, must be 128 x 128 pixels.\n";
		return false;
	}

	return true;
}

void setPixel(int x, int y, uint16_t color) {
	if (x < 0 || x >= 128 || y < 0 || y >= 128) {
		return;
	}

	//making 0,0 correspond to top left, not bottom left
	y = 127 - y;

	for (int sy = 0; PIXEL_SCALE > sy; sy++) {
		for (int sx = 0; PIXEL_SCALE > sx; sx++) {
			frameBuffer[3 * ((PIXEL_SCALE * y + sy) * (128 * PIXEL_SCALE) + x * PIXEL_SCALE + sx)] = (color & 0xF800) >> 8; //red
			frameBuffer[3 * ((PIXEL_SCALE * y + sy) * (128 * PIXEL_SCALE) + x * PIXEL_SCALE + sx) + 1] = (color & 0x7E0) >> 3; //green
			frameBuffer[3 * ((PIXEL_SCALE * y + sy) * (128 * PIXEL_SCALE) + x * PIXEL_SCALE + sx) + 2] = (color & 0x1F) << 3; //blue
		}
	}
}

uint16_t getPixel(int x, int y) {
	if (x < 0 || x >= 128 || y < 0 || y >= 128) {
		return 0;
	}

	//making 0,0 correspond to top left, not bottom left
	y = 127 - y;

	//will assume all pixels of the scaled pixel are the same, so will only check one
	return frameBuffer[3 * ((PIXEL_SCALE * y) * (128 * PIXEL_SCALE) + x * PIXEL_SCALE)];
}

void clearScreen() {
	for (int y = 0; 128 > y; y++) {
		for (int x = 0; 128 > x; x++) {
			setPixel(x, y, backgroundColor);
		}
	}
}

void drawCharacter(char c) {
	//assumes the position and attributes have already been set

	if (c < 32 || c > 126) {
		return; //any character outside this range does not have a mapping
	}

	int iIndex = c - 32; //preparing to read the character from the image
	int row = iIndex / 18;
	int col = iIndex % 18;

	uint16_t onColor = textForeground;
	uint16_t offColor = textBackground;

	if (textInvertColors) {
		onColor = textBackground;
		offColor = textForeground;
	}

	//starts from lower right corner, right because of how bolding works
	for (int y = 7; 0 <= y; y--) {
		for (int x = 7; 0 <= x; x--) {
			//reading from the font image, the result is grayscale
			unsigned char v = fontImage[(row * 8 + y) * 128 + (col * 7 + x)];

			//testing if the value, v, is within the acceptable range for an 'on' pixel of 0-50
			bool isOn = v <= 50;
			uint16_t pColor = (textUnderline && y == 7) || isOn ? onColor : offColor;

			//font could have a width/height scalar
			for (int sy = 0; textYMultiplier > sy; sy++) {
				for (int sx = 0; textXMultiplier > sx; sx++) {
					int dispX = (textXPos * 7 + x * textXMultiplier) + sx;
					int dispY = (textYPos * 8 + y * textYMultiplier) + sy;

					if (textItalic) {
						//slightly offset dispX
						dispX += ((textYPos * 8 + 7 * textXMultiplier) - dispY + textYMultiplier) / 2;
					}

					if (textBold && isOn) {
						//must write to both the current pixel and the adjacent one to the right
						setPixel(dispX, dispY, pColor);
						setPixel(dispX + textXMultiplier, dispY, pColor);
					}
					else {
						setPixel(dispX, dispY, pColor);
					}
				}
			}
		}
	}

	textXPos += textXMultiplier;
}

void drawCircle(int xc, int yc, int radius, uint16_t color, bool filled) {

	int rSq = radius * radius - 2; //the minus two is to prevent tiny parts of the circle from protruding where x == xc and y == yc
	uint16_t colorOutline = color;
	if (filled && outlineColor != 0) {
		colorOutline = outlineColor;
	}

	for (int y = yc + originY - radius; yc + originY + radius >= y; y++) {
		for (int x = xc + originX - radius; xc + originX + radius >= x; x++) {
			//calculating the distance from the center
			int dx = x - (xc + originX);
			int dy = y - (yc + originY);
			int distSq = dx * dx + dy * dy;

			//testing if the point is in the circle
			if (distSq > rSq) {
				continue;
			}

			//testing if the point is on the outline
			if (rSq - distSq < 2 * radius) {
				//on the outline
				setPixel(x, y, colorOutline);
				continue;
			}

			if (!filled) {
				continue;
			}

			setPixel(x, y, color);
		}
	}
}

int linePosX(short int color, VERTEX v1, VERTEX v2) {
	int dx = v2.x - v1.x;
	int dy = v2.y - v1.y;
	int err = 0;
	int ly = v1.y;
	int ret = 1;
	for (int lx = v1.x; v2.x > lx; lx++) {
		setPixel(lx, ly, color);
		if (2 * (err + dy) < dx) {
			err += dy;
		}
		else {
			ly += 1;
			err += dy - dx;
		}
		ret++;
	}
	return ret;
}

int linePosY(short int color, VERTEX v1, VERTEX v2) {
	int dx = v2.x - v1.x;
	int dy = v2.y - v1.y;
	int err = 0;
	int lx = v1.x;
	int ret = 1;
	for (int ly = v1.y; v2.y > ly; ly++) {
		setPixel(lx, ly, color);
		if (2 * (err + dx) < dy) {
			err += dx;
		}
		else {
			lx += 1;
			err += dx - dy;
		}
		ret++;
	}
	return ret;
}

int lineNegX(short int color, VERTEX v1, VERTEX v2) {
	int dx = v2.x - v1.x;
	int dy = v2.y - v1.y;
	int err = 0;
	int ly = v1.y;
	int ret = 1;
	for (int lx = v1.x; v2.x > lx; lx++) {
		setPixel(lx, ly, color);
		if (2 * (err + dy) > 0 - dx) {
			err += dy;
		}
		else {
			ly -= 1;
			err += dy + dx;
		}
		ret++;
	}
	return ret;
}

int lineNegY(short int color, VERTEX v1, VERTEX v2) {
	int dx = v2.x - v1.x;
	int dy = v2.y - v1.y;
	int err = 0;
	int lx = v2.x;
	int ret = 1;
	for (int ly = v2.y; v1.y < ly; ly--) {
		setPixel(lx, ly, color);
		if (2 * (err + dx) > 0 - dy) {
			err += dx;
		}
		else {
			lx -= 1;
			err += dx + dy;
		}
		ret++;
	}
	return ret;
}

bool drawLine(VERTEX v1, VERTEX v2, uint16_t color) {

	//adjusting for origin position
	v1.x += originX;
	v2.x += originX;
	v1.y += originY;
	v2.y += originY;

	if (v2.x < v1.x && v2.y < v1.y) {
		//Third quadrant
		VERTEX temp = v2;
		v2 = v1;
		v1 = temp;
	}
	else if (v2.x < v1.x && v2.y > v1.y) {
		//Second quadrant
		VERTEX temp = v2;
		v2 = v1;
		v1 = temp;
	}

	//Calculates the number of cycles needed to execute the command, unused for this program
	int cycles = 0;

	if (v2.y - v1.y > 0) {
		//First quadrant
		if ((v2.y - v1.y) - (v2.x - v1.x) > 0) {
			//m > 1
			cycles = linePosY(color, v1, v2);
		}
		else {
			//m < 1
			cycles = linePosX(color, v1, v2);
		}
	}
	else {
		//Forth quadrant
		if ((v2.y - v1.y) + (v2.x - v1.x) > 0) {
			//m < -1
			cycles = lineNegY(color, v1, v2);
		}
		else {
			//m > -1
			cycles = lineNegX(color, v1, v2);
		}
	}

	return false;
}

void drawRectangle(int sx, int sy, int width, int height, uint16_t color, bool filled) {
	uint16_t colorOutline = color;
	if (filled && outlineColor != 0) {
		colorOutline = outlineColor;
	}

	for (int y = sy + originY; sy + originY + height > y; y++) {
		for (int x = sx + originX; sx + originX + width > x; x++) {
			
			//testing if the pixel is on the outline
			if (x == sx + originX || y == sy + originY || x == sx + originX + width - 1 || y == sy + originY + height - 1) {
				setPixel(x, y, colorOutline);
				continue;
			}

			if (!filled) {
				continue;
			}

			setPixel(x, y, color);
		}
	}
}

int16_t readWord() {
	int16_t ret;
	unsigned char temp;
	DWORD read = 0;
	if (com) {
		ReadFile(com, &temp, 1, &read, NULL);
	}
	else if (ftCom) {
		FT_Read(ftCom, &temp, 1, &read);
	}
	if (read == 0) {
		timedout = true;
		return 0;
	}
	ret = temp;
	if (com) {
		ReadFile(com, &temp, 1, &read, NULL);
	}
	else if (ftCom) {
		FT_Read(ftCom, &temp, 1, &read);
	}
	if (read == 0) {
		timedout = true;
		return 0;
	}
	ret = (ret << 8) | temp;
	//std::cout << "read word: " << (uint16_t)ret << std::endl;
	return ret;
}

uint8_t readChar() {
	unsigned char temp;
	DWORD read = 0;
	if (com) {
		ReadFile(com, &temp, 1, &read, NULL);
	}
	else if (ftCom) {
		FT_Read(ftCom, &temp, 1, &read);
	}
	if (read == 0) {
		timedout = true;
		return 0;
	}
	//std::cout << "read char: " << (uint16_t)temp << std::endl;
	return temp;
}

void runDisplayLoop() {
	std::cout << "started uart scanner\n";

	PurgeComm(com, PURGE_RXCLEAR);

	while (running) {
		timedout = false;
		uint16_t cmd = readWord();

		if (cmd == 0 && !timedout) {
			//most likely from a reset, so will reset the software screen.
			//will keep looping until a non-zero value is found (which must be from after the serial rate is synced)
			//the first command sent should be a clear screen command for this to work properly
			while (running) {
				uint8_t temp = readChar();

				if (temp != 0) {
					cmd = temp << 8;
					temp = readChar();
					cmd |= temp;

					//resetting the display
					textXPos = 0;
					textYPos = 0;
					textUnderline = false;
					textBold = false;
					textItalic = false;
					textInvertColors = false;
					backgroundColor = 0;
					textForeground = 0xFFFF;
					textBackground = 0;
					timedout = false;
					textXMultiplier = 1;
					textYMultiplier = 1;
					originX = 0;
					originY = 0;

					clearScreen();

					break;
				}
			}
		}

		int16_t p1, p2, p3, p4, p5, p6, p7; //because C doesn't count each case as a separate scope to prevent naming conflicts, so I have made them generic in name so I can reuse
		int16_t lowestX;
		int16_t lowestY;
		int16_t width;
		int16_t height;
		DWORD read;
		char strchar, putChar;
		char blitBuf[256];

		if (!timedout) {
			//A command

			//std::cout << "received command: " << cmd << std::endl;
			switch (cmd) {
			case 0xFFD7:
				//clear screen
				clearScreen();
				break;
			case 0xFFCD:
				//draw circle no fill
				p1 = readWord(); //x
				if (timedout) break;
				p2 = readWord(); //y
				if (timedout) break;
				p3 = readWord(); //radius
				if (timedout) break;
				p4 = readWord(); //color
				drawCircle(p1, p2, p3, p4, false);
				break;
			case 0xFFCC:
				//draw filled circle
				p1 = readWord(); //x
				if (timedout) break;
				p2 = readWord(); //y
				if (timedout) break;
				p3 = readWord(); //radius
				if (timedout) break;
				p4 = readWord(); //color
				drawCircle(p1, p2, p3, p4, true);
				break;
			case 0xFFD2:
				//draw line
				p1 = readWord(); //x1
				if (timedout) break;
				p2 = readWord(); //y1
				if (timedout) break;
				p3 = readWord(); //x2
				if (timedout) break;
				p4 = readWord(); //y2
				if (timedout) break;
				p5 = readWord(); //color
				drawLine({ p1, p2 }, { p3, p4 }, p5);
				break;
			case 0xFFCF:
				//draw rectangle no fill
				p1 = readWord(); //x1
				if (timedout) break;
				p2 = readWord(); //y1
				if (timedout) break;
				p3 = readWord(); //x2
				if (timedout) break;
				p4 = readWord(); //y2
				if (timedout) break;
				p5 = readWord(); //color

				//converting coordinate pairs to x, y, width, height
				lowestX = p1 < p3 ? p1 : p3;
				lowestY = p2 < p4 ? p2 : p4;
				width = abs(p1 - p3) + 1;
				height = abs(p2 - p4) + 1;

				drawRectangle(lowestX, lowestY, width, height, p5, false);

				break;
			case 0xFFCE:
				//draw filled rectangle
				p1 = readWord(); //x1
				if (timedout) break;
				p2 = readWord(); //y1
				if (timedout) break;
				p3 = readWord(); //x2
				if (timedout) break;
				p4 = readWord(); //y2
				if (timedout) break;
				p5 = readWord(); //color

				//converting coordinate pairs to x, y, width, height
				lowestX = p1 < p3 ? p1 : p3;
				lowestY = p2 < p4 ? p2 : p4;
				width = abs(p1 - p3) + 1;
				height = abs(p2 - p4) + 1;

				drawRectangle(lowestX, lowestY, width, height, p5, true);
				break;
			case 0x0005:
				//draw poly line

				break;
			case 0x0004:
				//draw polygon no fill

				break;
			case 0xFFC9:
				//draw triangle no fill
				p1 = readWord(); //x1
				if (timedout) break;
				p2 = readWord(); //y1
				if (timedout) break;
				p3 = readWord(); //x2
				if (timedout) break;
				p4 = readWord(); //y2
				if (timedout) break;
				p5 = readWord(); //x3
				if (timedout) break;
				p6 = readWord(); //y3
				if (timedout) break;
				p7 = readWord(); //color

				//since triangles can only have no fill, will just use the line drawer
				drawLine({ p1, p2 }, { p3, p4 }, p7);
				drawLine({ p3, p4 }, { p5, p6 }, p7);
				drawLine({ p5, p6 }, { p1, p2 }, p7);

				break;
			case 0xFFCB:
				//put pixel
				p1 = readWord(); //x
				if (timedout) break;
				p2 = readWord(); //y
				if (timedout) break;
				p3 = readWord(); //color
				setPixel(p1, p2, p3);
				break;
			case 0xFFD6:
				//move origin
				originX = readWord(); //x pos
				if (timedout) break;
				originY = readWord(); //y pos
				break;
			case 0xFFD4:
				//draw line and move origin
				p1 = readWord(); //x1
				if (timedout) break;
				p2 = readWord(); //y1
				if (timedout) break;
				p3 = readWord(); //x2
				if (timedout) break;
				p4 = readWord(); //y2
				if (timedout) break;
				p5 = readWord(); //color
				drawLine({ p1, p2 }, { p3, p4 }, p5);
				originX = p3;
				originY = p4;
				break;
			case 0xFF6C:
				//toggle clipping
				p1 = readWord();
				//unimplemented for now
				break;
			case 0xFFBF:
				//set clipping window
				p1 = readWord(); //x1
				if (timedout) break;
				p2 = readWord(); //y1
				if (timedout) break;
				p3 = readWord(); //x2
				if (timedout) break;
				p4 = readWord(); //y2

				//unimplemented for now
				break;
			case 0xFF6E:
				//set background color
				backgroundColor = readWord();
				break;
			case 0xFF67:
				//set outline color
				outlineColor = readWord();
				break;
			case 0xFF65:
				//set line pattern
				p1 = readWord();
				//unimplemented for now
				break;
			case 0xFFD5:
				//set graphics parameters
				p1 = readWord(); //function
				if (timedout) break;
				p2 = readWord(); //value
				//unimplemented for now
				break;
			case 0x000B:
				//change serial baud rate
				p1 = readWord();
				//doesn't do anything because then the application would need to be restarted every time the display is reset.
				break;
			case 0x000A:
				//BLIT
				p1 = readWord(); //x
				if (timedout) break;
				p2 = readWord(); //y
				if (timedout) break;
				p3 = readWord(); //width
				if (timedout) break;
				p4 = readWord(); //height
				if (timedout) break;

				p5 = 0; //a counter
				p6 = p3 * p4;
				for (int y = p2; p2 + p4 > y; y++) {
					for (int x = p1; p1 + p3 > x; x++) {
						/*if (p5 % 128 == 0) {
							read = 0;
							p7 = (p6 - p5 >= 128 ? 128 : p6 - p5) << 1;
							ReadFile(com, blitBuf, p7, &read, NULL);
							if (read != p7) {
								timedout = true;
							}
						}*/
						p7 = readWord(); //color
						if (timedout) break;
						setPixel(x, y, p7);
						//setPixel(x, y, (blitBuf[(p5 % 128) << 1] << 8) | blitBuf[((p5 % 128) << 1) + 1]);
						p5++;
					}
					if (timedout) break;
				}

				break;
			case 0xFFE4:
				//move text cursor
				textYPos = readWord() * textXMultiplier; //line
				if (timedout) break;
				textXPos = readWord() * textYMultiplier; //column
				break;
			case 0xFFFE:
				//put character
				putChar = readWord(); //idk why a word, but that's what it is.
				drawCharacter(putChar);
				textBold = false;
				textUnderline = false;
				textItalic = false;
				textInvertColors = false;
				break;
			case 0x0006:
				//put string (null terminated)
				strchar = readChar();
				p1 = textXPos;
				while (strchar != 0 && !timedout) {
					if (strchar == '\n') {
						textXPos = p1;
						textYPos += textYMultiplier;
					}
					drawCharacter(strchar);

					strchar = readChar();
				};
				textBold = false;
				textUnderline = false;
				textItalic = false;
				textInvertColors = false;
				break;
			case 0xFF7F:
				//set text foreground color
				textForeground = readWord();
				break;
			case 0xFF7E:
				//set text background color
				textBackground = readWord();
				break;
			case 0xFF7C:
				//set text width multiplier
				textXMultiplier = readWord();
				break;
			case 0xFF7B:
				//set text height multiplier
				textYMultiplier = readWord();
				break;
			case 0xFF7A:
				//set text X spacing
				textXSpacing = readWord();
				break;
			case 0xFF79:
				//set text Y spacing
				textYSpacing = readWord();
				break;
			case 0xFF76:
				//toggle text bold (self resets)
				p1 = readWord();
				textBold = p1 == 1;
				break;
			case 0xFF74:
				//toggle text color inverse (self resets)
				p1 = readWord();
				textInvertColors = p1 == 1;
				break;
			case 0xFF75:
				//toggle text italic (self resets)
				p1 = readWord();
				textItalic = p1 == 1;
				break;
			case 0xFF77:
				//toggle text opacity

				break;
			case 0xFF73:
				//toggle text underline (self resets)
				p1 = readWord();
				textUnderline = p1 == 1;
				break;
			case 0xFF7D:
				//set font
				//just here for compliance, but doesn't do anything
				p1 = readWord();
				break;
			default:
				std::cout << "Invalid command recieved: " << cmd << std::endl;
			}


		}

	}
}