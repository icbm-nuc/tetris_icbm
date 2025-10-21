#include <stdio.h>
#include <Windows.h> // Windows API
#include <conio.h>
#include "block.h"
#include <time.h>
#include <stdlib.h>



#define LEFT_KEY 75
#define RIGHT_KEY 77
#define UP_KEY 72
#define DOWN_KEY 80
#define NUM_OF_BLOCKS 7

#define STAGE_WIDTH 24
#define STAGE_HEIGHT 20

int curX = 0;
int curY = 0;
int curBlockIdx = 0;
int stageInfo[STAGE_HEIGHT + 1][STAGE_WIDTH + 2] = { 0, };


void setCursorPosition(int x, int y) {

	COORD pos = { x, y };
	HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOut, pos);
}
void eraseCursor() {
	HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO  curCursorInfo;
	GetConsoleCursorInfo(hConsoleOut, &curCursorInfo);
	curCursorInfo.bVisible = 0;
	SetConsoleCursorInfo(hConsoleOut, &curCursorInfo);
}

void drawBlock(int x, int y, int idx) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (blockModel[idx][i][j]) {
				setCursorPosition(2 * (x + j), y + i);
				printf("■");

			}
		}
	}
}

void eraseBlock(int x, int y, int idx) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (blockModel[idx][i][j]) {
				setCursorPosition(2 * (x + j), y + i);
				printf("  ");
			}
		}
	}
}


int detectCollision(int x, int y, int idx) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (blockModel[idx][i][j]) {
				if (stageInfo[y + i][2 * (x + j)] >= 1)
					return 1;
				if (stageInfo[y + i][2 * (x + j) + 1] >= 1)
					return 1;
			}
		}
	}
	return 0;
}
void initBlock() {
	curX = 5;
	curY = 0;
	srand(time(NULL));
	curBlockIdx = (rand() % NUM_OF_BLOCKS) * 4;
	if (detectCollision(curX, curY, curBlockIdx))
		printf("Game Over");
	return ;
	drawBlock(curX, curY, curBlockIdx);
}

int setBlockConcrete(int x, int y, int idx) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (blockModel[idx][i][j]) {
				stageInfo[y + i][2 * (x + j)] = 2;
				stageInfo[y + i][2 * (x + j) + 1] = 2;
			}
		}
	}
	return 0;
}

int moveBlock(int x, int y) {
	if (detectCollision(curX + x, curY + y, curBlockIdx))
		return 0;

	eraseBlock(curX, curY, curBlockIdx);
	curX += x;
	curY += y;
	drawBlock(curX, curY, curBlockIdx);
	return 1;
}

void rotateBlock() {
	int nextIdx = curBlockIdx + 1;
	if (nextIdx == 4) {
		nextIdx = 0;
	}
	if (detectCollision(curX, curY, nextIdx))
		return;

	eraseBlock(curX, curY, curBlockIdx);
	if (curBlockIdx % 4 == 3) {
		curBlockIdx -= 3;
	}
	else {
		curBlockIdx += 1;
	}
	drawBlock(curX, curY, curBlockIdx);
}

void drawStage() {
	for (int y = 0; y < STAGE_HEIGHT; y++) {
		setCursorPosition(0, y);
		printf("│ ");
		stageInfo[y][0] = 1;
		stageInfo[y][1] = 1;
		setCursorPosition(STAGE_WIDTH, y);
		printf("│ ");
		stageInfo[y][STAGE_WIDTH] = 1;
		stageInfo[y][STAGE_WIDTH + 1] = 1;
	}
	for (int x = 1; x < STAGE_WIDTH; x++) {
		setCursorPosition(x, STAGE_HEIGHT);
		printf("─");
		stageInfo[STAGE_HEIGHT][x] = 1;
	}
	setCursorPosition(0, STAGE_HEIGHT);
	printf("└");
	stageInfo[STAGE_HEIGHT][0] = 1;
	setCursorPosition(STAGE_WIDTH, STAGE_HEIGHT);
	printf("┘");
	stageInfo[STAGE_HEIGHT][STAGE_WIDTH] = 1;
	stageInfo[STAGE_HEIGHT][STAGE_WIDTH + 1] = 1;
}

void testStage() {
	for (int y = 0; y < STAGE_HEIGHT + 1; y++) {
		for (int x = 0; x < STAGE_WIDTH + 2; x++) {
			setCursorPosition(x + STAGE_HEIGHT + 10, y);
			printf("%d", stageInfo[y][x]);
		}
	}
}

void processKeyInput() {
	int key = 0;
	if (_kbhit() != 0) {
		key = _getch();
	}
	switch (key) {
	case LEFT_KEY:
		moveBlock(-1, 0);
		break;
	case RIGHT_KEY:
		moveBlock(1, 0);
		break;
	case UP_KEY:
		rotateBlock();
		break;
	case DOWN_KEY:
		moveBlock(0, 1);
		break;
	}
}
void drawConcreteBlocks() {
	for (int y = 0; y < STAGE_HEIGHT; y++) {
		for (int x = 2 / 2; x < (STAGE_WIDTH) / 2; x++) {
			setCursorPosition(2 * x, y);
			if (stageInfo[y][2 * x] > 1) {
				printf("■");
			}
			else {
				printf("  ");
			}
		}
	}
}
void removeFilledBlock() {
	for (int y = STAGE_HEIGHT + 1; y >= 0; y--) {
		int cnt = 0;
		for (int x = 0; x < STAGE_WIDTH + 2; x++) {
			if (stageInfo[y][x] == 2)
				cnt++;
		}
		if (cnt == STAGE_WIDTH - 2) {
			for (int y1 = y; y1 >= 1; y1--) {
				for (int x = 2; x < STAGE_WIDTH; x++) {
					stageInfo[y1][x] = stageInfo[y1 - 1][x];
				}
			}
			for (int x = 2; x < STAGE_WIDTH; x++) {
				stageInfo[0][x] = 0;
			}
			y++;
		}
	}
	drawConcreteBlocks();
}

int main(void) {
	eraseCursor();
	drawStage();
	initBlock();
	while (1) {
		//testStage();
		for (int i = 0; i < 100; i++) {
			processKeyInput();
			Sleep(10);
		}
		if (moveBlock(0, 1) == 0) {
			setBlockConcrete(curX, curY, curBlockIdx);
			removeFilledBlock();
			initBlock();
		}
		Sleep(10);
	}
	return 0;
}


