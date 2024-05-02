#pragma once
#include "Screen.h"

struct pos {
    int x;
    int y;
};

class CellAuto {
private:
    int x;
    int y;
    int width;
    int height;
    bool** cells;

public:
    CellAuto();

    CellAuto(const CellAuto& cellAuto);

    CellAuto(int x, int y, int width, int height);

    ~CellAuto();

    pos getMouseGridPos(int mouseX, int mouseY);

    bool toggleCell(int gridPosX, int gridPosY);

    bool toggleCell(pos gridPos);

    int getCellState(pos gridPos);

    int getNeighboursAlive(pos gridPos);

    void updateCells();

    void display(Screen& screen);
};