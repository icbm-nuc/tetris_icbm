#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "block.h"

#define MAX_WIDTH 40
#define MAX_HEIGHT 40

int stageInfo[MAX_HEIGHT][MAX_WIDTH] = { 0 }; // 실제 보드
int stageInfoTwo[MAX_HEIGHT][MAX_WIDTH] = { 0 }; // 우측블록 표시용

// 다음 블럭을 오른쪽에 표시하는 함수
void showNextBlock(int next_block_type, int board_width,int board_height) {
    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    int offsetX = 5 ; // 충분히 오른쪽에 위치
    int offsetY = board_height + 7; // 원하는 높이

    // "NEXT" 타이틀 출력
    COORD labelPos = { offsetX, offsetY - 2 };
    SetConsoleCursorPosition(hConsoleOut, labelPos);
    printf("NEXT Block");

    // 블록 출력
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            COORD pos = { offsetX + j * 2, offsetY + i };
            SetConsoleCursorPosition(hConsoleOut, pos);
            if (blockModel[next_block_type * 4][i][j]) {
                printf("■");
            }
            else {
                printf("  ");
            }
        }
    }
}

bool detectCollision(int x, int y, int idx, int in_width, int in_height) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (blockModel[idx][i][j]) {
                int nx = x + j;
                int ny = y + i;
                if (nx < 0 || nx >= in_width || ny < 0 || ny >= in_height)
                    return true;
                if (stageInfo[ny][nx] == 1 || stageInfo[ny][nx] == 2)
                    return true;
            }
        }
    }
    return false;
}

void fixBlock(int x, int y, int idx) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (blockModel[idx][i][j]) {
                stageInfo[y + i][x + j] = 1;
            }
        }
    }
}

void fixBlockTwo(int x, int y, int idx) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (blockModel[idx][i][j]) {
                stageInfoTwo[y + i][x + j] = 2;
            }
        }
    }
}

void removeFullLines(int in_width, int in_height) {
    for (int row = in_height - 2; row >= 0; row--) {
        bool full = true;
        for (int col = 1; col < in_width - 1; col++) {
            if (stageInfo[row][col] == 0) {
                full = false;
                break;
            }
        }
        if (full) {
            for (int moveRow = row; moveRow > 0; moveRow--) {
                for (int col = 1; col < in_width - 1; col++) {
                    stageInfo[moveRow][col] = stageInfo[moveRow - 1][col];
                }
            }
            for (int col = 1; col < in_width - 1; col++) {
                stageInfo[0][col] = 0;
            }
            row++;
        }
    }
}

void removeFullLinesTwo(int in_width, int in_height) {
    for (int row = in_height - 2; row >= 0; row--) {
        bool full = true;
        for (int col = 1; col < in_width - 1; col++) {
            if (stageInfoTwo[row][col] == 0) {
                full = false;
                break;
            }
        }
        if (full) {
            for (int moveRow = row; moveRow > 0; moveRow--) {
                for (int col = 1; col < in_width - 1; col++) {
                    stageInfoTwo[moveRow][col] = stageInfoTwo[moveRow - 1][col];
                }
            }
            for (int col = 1; col < in_width - 1; col++) {
                stageInfoTwo[0][col] = 0;
            }
            row++;
        }
    }
}

void redrawStage(int in_width, int in_height) {
    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int y = 0; y < in_height; y++) {
        for (int x = 0; x < in_width; x++) {
            COORD pos = { 2 * x, y + 3 };
            SetConsoleCursorPosition(hConsoleOut, pos);
            if (stageInfo[y][x] == 1) {
                printf("■");
            }
            else if (stageInfo[y][x] == 2) {
                if (x == 0 || x == in_width - 1) {
                    printf("│");
                }
                else if (y == in_height - 1) {
                    printf("─");
                }
                else {
                    printf("  ");
                }
            }
            else {
                printf("  ");
            }
        }
    }
}

void redrawStageTwo(int in_width, int in_height) {
    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    int offsetX = (in_width * 2) + 5;
    for (int y = 0; y < in_height; y++) {
        for (int x = 0; x < in_width; x++) {
            COORD pos = { offsetX + x, y + 3 };
            SetConsoleCursorPosition(hConsoleOut, pos);
            if (stageInfoTwo[y][x] == 1) {
                printf("1");
            }
            else if (stageInfoTwo[y][x] == 2) {
                printf("2");
            }
            else {
                printf("0");
            }
        }
    }
}

double getCurrentTime() {
    return (double)GetTickCount64();
}

void drawBlock(int x, int y, int type) {
    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (blockModel[type][i][j]) {
                COORD pos = { 2 * (x + j), (y + i) + 3 };
                SetConsoleCursorPosition(hConsoleOut, pos);
                printf("■");
            }
        }
    }
}

void drawBlockTwo(int x, int y, int type, int in_width) {
    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    int offsetX = (in_width * 2) + 5;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (blockModel[type][i][j]) {
                COORD pos = { offsetX + (x + j), (y + i) + 3 };
                SetConsoleCursorPosition(hConsoleOut, pos);
                printf("2");
            }
        }
    }
}

void clearBlock(int x, int y, int type) {
    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (blockModel[type][i][j]) {
                COORD pos = { 2 * (x + j), (y + i) + 3 };
                SetConsoleCursorPosition(hConsoleOut, pos);
                printf("  ");
            }
        }
    }
}

void clearBlockTwo(int x, int y, int type, int in_width) {
    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    int offsetX = (in_width * 2) + 5;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (blockModel[type][i][j]) {
                COORD pos = { offsetX + (x + j), (y + i) + 3 };
                SetConsoleCursorPosition(hConsoleOut, pos);
                printf("0");
            }
        }
    }
}

void createStage(int in_width, int in_height) {
    for (int i = 0; i < in_height; i++) {
        stageInfo[i][0] = 2;
        stageInfo[i][in_width - 1] = 2;
    }
    for (int i = 0; i < in_width; i++) {
        stageInfo[in_height - 1][i] = 2;
    }
}

void createStageTwo(int in_width, int in_height) {
    for (int i = 0; i < in_height; i++) {
        stageInfoTwo[i][0] = 1;
        stageInfoTwo[i][in_width - 1] = 1;
    }
    for (int i = 0; i < in_width; i++) {
        stageInfoTwo[in_height - 1][i] = 1;
    }
}

int main() {
    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsoleOut, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsoleOut, &cursorInfo);

    int in_width, in_height;
    int x = 0, y = 0;
    int block_type = 0;
    int in_arrow_key = 0;
    int r = 0;
    int next_block_type;

    printf("가로 크기 입력 : ");
    if (scanf("%d", &in_width) != 1) return 1;
    printf("세로 크기 입력 : ");
    if (scanf("%d", &in_height) != 1) return 1;

    srand((unsigned int)time(NULL));
    next_block_type = rand() % (sizeof(blockModel) / sizeof(blockModel[0]) / 4);
    next_block_type *= 4;

    x = in_width / 2 - 2;
    y = 0;
    double lastFallTime = getCurrentTime();

    createStage(in_width, in_height);
    createStageTwo(in_width, in_height);

    redrawStage(in_width, in_height);
    redrawStageTwo(in_width, in_height);

    // ---- 다음 블럭 최초 표시 ----
    showNextBlock(next_block_type / 4, in_width,in_height);

    block_type = next_block_type;
    int current_block_base = block_type / 4;

    while (1) {
        double start = getCurrentTime();

        clearBlock(x, y, block_type);
        clearBlockTwo(x, y, block_type, in_width);

        if (_kbhit()) {
            int key = _getch();
            if (key == 224) key = _getch();
            int next_x = x, next_y = y, temp_arrow = in_arrow_key;
            switch (key) {
            case 72:
                temp_arrow = (in_arrow_key + 1) % 4;
                break;
            case 75:
                next_x--;
                break;
            case 77:
                next_x++;
                break;
            case 80:
                next_y++;
                break;
            }
            int temp_type = current_block_base * 4 + temp_arrow;
            if (!detectCollision(next_x, next_y, temp_type, in_width, in_height)) {
                x = next_x;
                y = next_y;
                in_arrow_key = temp_arrow;
                block_type = temp_type;
            }
        }

        double now = getCurrentTime();
        if (now - lastFallTime >= 1000.0) {
            int next_y = y + 1;
            if (!detectCollision(x, next_y, block_type, in_width, in_height)) {
                y = next_y;
            }
            else {
                fixBlock(x, y, block_type);
                fixBlockTwo(x, y, block_type);
                removeFullLines(in_width, in_height);
                removeFullLinesTwo(in_width, in_height);

                redrawStage(in_width, in_height);
                redrawStageTwo(in_width, in_height);

                // 새 블럭 준비 (다음 블럭을 현재 블럭으로)
                current_block_base = next_block_type / 4;
                x = in_width / 2 - 2;
                y = 0;
                in_arrow_key = 0;
                block_type = current_block_base * 4 + in_arrow_key;

                // 새로운 next_block_type 선정
                next_block_type = rand() % (sizeof(blockModel) / sizeof(blockModel[0]) / 4);
                next_block_type *= 4;

                // ---- 다음 블럭 다시 표시 ----
                showNextBlock(next_block_type / 4, in_width,in_height);

                if (detectCollision(x, y, block_type, in_width, in_height))
                    break;
            }
            lastFallTime = now;
        }

        drawBlock(x, y, block_type);
        drawBlockTwo(x, y, block_type, in_width);

        double elapsed = getCurrentTime() - start;
        double target = 10.0f;
        if (elapsed < target) Sleep((target - elapsed));
    }
    return 0;
}
