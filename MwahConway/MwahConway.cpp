#include <iostream>
#include <windows.h>
#include "Screen.h"

struct pos {
    int x;
    int y;
};

//Cellular Automaton
class CellAuto {
private:
    int x;
    int y;
    int width;
    int height;
    bool cells[20][20];

public:
    CellAuto() {
        x = 0;
        y = 0;
        width = 20;
        height = 20;

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                cells[i][j] = false;
            }
        }
    }

    CellAuto(const CellAuto& cellAuto) {
        x = cellAuto.x;
        y = cellAuto.y;
        width = cellAuto.width;
        height = cellAuto.height;

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                cells[i][j] = cellAuto.cells[i][j];
            }
        }
    }

    CellAuto(int x, int y) {
        this->x = x;
        this->y = y;
        width = 20;
        height = 20;

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                cells[i][j] = false;
            }
        }
    }

    //returns {-1, -1} if out of cell grid
    pos getMouseGridPos(int mouseX, int mouseY) {
        if (mouseX < x || mouseX > x + (width * 2) - 1 || mouseY < y || mouseY > y + height - 1) {
            return { -1, -1 };
        }

        return { (mouseX - x) / 2, mouseY - y };
    }

    void toggleCell(int cellPosX, int cellPosY) {
        cells[cellPosY][cellPosX] = !cells[cellPosY][cellPosX];
    }

    void toggleCell(pos cellPos) {
        cells[cellPos.y][cellPos.x] = !cells[cellPos.y][cellPos.x];
    }

    void display(Screen& screen) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (cells[i][j]) {
                    screen.input(219, x + (j * 2), y + i);
                    screen.input(219, x + 1 + (j * 2), y + i);
                    continue;
                }

                screen.input('[', x + (j * 2), y + i);
                screen.input(']', x + 1 + (j * 2), y + i);
            }
        }
    }
};

int main() {
    SetConsoleOutputCP(437);

    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);

    DWORD consoleMode;
    GetConsoleMode(hout, &consoleMode);
    SetConsoleMode(hout, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    SetConsoleMode(hin, ENABLE_EXTENDED_FLAGS | ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);

    GetCurrentConsoleFontEx(hout, false, &cfi);

    cfi.dwFontSize.Y = 20;
    cfi.dwFontSize.X = 11;

    SetCurrentConsoleFontEx(hout, false, &cfi);

    SHORT width = 200;
    SHORT height = 50;

    SMALL_RECT dim = { 0, 0, width - 1, height - 1 };

    SetConsoleScreenBufferSize(hout, { width, height });
    SetConsoleWindowInfo(hout, true, &dim);

    INPUT_RECORD inputRecord;
    DWORD events;

    Screen screen = Screen(width, height);

    //game ticks per second
    int tickrate = 50;

    int mouseX = 0;
    int mouseY = 0;
    
    CellAuto cellAuto = CellAuto(50, 10);
    cellAuto.toggleCell(4, 1);

    while (true) {
        GetNumberOfConsoleInputEvents(hin, &events);

        if (events > 0) {
            ReadConsoleInput(hin, &inputRecord, 1, &events);
            switch (inputRecord.EventType) {
            case MOUSE_EVENT:
                mouseX = inputRecord.Event.MouseEvent.dwMousePosition.X;
                mouseY = inputRecord.Event.MouseEvent.dwMousePosition.Y;

                if (inputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                    cellAuto.toggleCell(cellAuto.getMouseGridPos(mouseX, mouseY));
                }
                break;
            }
        }

        screen.reset();

        cellAuto.display(screen);

        //mouse cursor
        //screen.input('@', mouseX, mouseY);

        pos gridPos = cellAuto.getMouseGridPos(mouseX, mouseY);
        screen.text("Mouse screen pos: x:" + std::to_string(mouseX) + ", y:" + std::to_string(mouseY), 2, 1);
        screen.text("Mouse grid pos: x:" + std::to_string(gridPos.x) + ", y:" + std::to_string(gridPos.y), 2, 3);

        screen.print();

        Sleep(1000 / tickrate);
    }
}