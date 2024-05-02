#include "CellAuto.h"

CellAuto::CellAuto() {
    x = 0;
    y = 0;
    width = 0;
    height = 0;

    cells = new bool* [height];
    for (int i = 0; i < height; i++) {
        cells[i] = new bool[width];
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cells[i][j] = false;
        }
    }
}

CellAuto::CellAuto(const CellAuto& cellAuto) {
    x = cellAuto.x;
    y = cellAuto.y;
    width = cellAuto.width;
    height = cellAuto.height;

    cells = new bool* [height];
    for (int i = 0; i < height; i++) {
        cells[i] = new bool[width];
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cells[i][j] = cellAuto.cells[i][j];
        }
    }
}

CellAuto::CellAuto(int x, int y, int width, int height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    cells = new bool* [height];
    for (int i = 0; i < height; i++) {
        cells[i] = new bool[width];
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cells[i][j] = false;
        }
    }
}

CellAuto::~CellAuto() {
    for (int i = 0; i < height; i++) {
        delete[] cells[i];
    }
    delete[] cells;
}

//returns {-1, -1} if out of cell grid
pos CellAuto::getMouseGridPos(int mouseX, int mouseY) {
    if (mouseX < x || mouseX > x + (width * 2) - 1 || mouseY < y || mouseY > y + height - 1) {
        return { -1, -1 };
    }

    return { (mouseX - x) / 2, mouseY - y };
}

bool CellAuto::toggleCell(int gridPosX, int gridPosY) {
    if (gridPosX < 0 || gridPosY < 0) {
        return false;
    }

    cells[gridPosY][gridPosX] = !cells[gridPosY][gridPosX];
    return true;
}

bool CellAuto::toggleCell(pos gridPos) {
    if (gridPos.x < 0 || gridPos.y < 0) {
        return false;
    }

    cells[gridPos.y][gridPos.x] = !cells[gridPos.y][gridPos.x];
    return true;
}

//returns -1 if gridPos is out of cell grid bounds
int CellAuto::getCellState(pos gridPos) {
    if (gridPos.x < 0 || gridPos.x >(width - 1) || gridPos.y < 0 || gridPos.y >(height - 1)) {
        return -1;
    }

    return cells[gridPos.y][gridPos.x];
}

int CellAuto::getNeighboursAlive(pos gridPos) {
    int neighboursAlive = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == 1 && j == 1) {
                continue;
            }

            if (getCellState({ (gridPos.x - 1) + j, (gridPos.y - 1) + i }) == 1) {
                neighboursAlive++;
            }
        }
    }

    return neighboursAlive;
}

void CellAuto::updateCells() {
    bool** newCells;
    newCells = new bool* [height];
    for (int i = 0; i < height; i++) {
        newCells[i] = new bool[width];
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (!cells[i][j]) {
                if (getNeighboursAlive({ j, i }) == 3) {
                    newCells[i][j] = true;
                    continue;
                }

                newCells[i][j] = false;
                continue;
            }

            if (getNeighboursAlive({ j, i }) < 2 || getNeighboursAlive({ j, i }) > 3) {
                newCells[i][j] = false;
                continue;
            }

            newCells[i][j] = true;
        }
    }

    for (int i = 0; i < height; i++) {
        delete[] cells[i];
    }
    delete[] cells;

    cells = newCells;
}

void CellAuto::display(Screen& screen) {
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