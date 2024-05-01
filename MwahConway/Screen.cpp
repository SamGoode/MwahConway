#include "Screen.h"
#include <iostream>

Screen::Screen() {
    width = 0;
    height = 0;
    screenMatrix = nullptr;
    matrixSize = 0;
}

Screen::Screen(int width, int height) {
    this->width = width;
    this->height = height;
    matrixSize = (width + 1) * height;

    screenMatrix = new char[matrixSize];
    for (int i = 0; i < matrixSize; i++) {
        if (i % (this->width+1) == this->width) {
            screenMatrix[i] = '\n';
            continue;
        }

        screenMatrix[i] = ' ';
    }
    screenMatrix[matrixSize - 1] = 0;
}

Screen::~Screen() {
    delete[] screenMatrix;
}

Screen& Screen::operator=(const Screen& screen) {
    delete[] screenMatrix;

    width = screen.width;
    height = screen.height;
    matrixSize = screen.matrixSize;

    screenMatrix = new char[matrixSize];
    for (int i = 0; i < matrixSize; i++) {
        screenMatrix[i] = screen.screenMatrix[i];
    }
    screenMatrix[matrixSize - 1] = 0;

    return *this;
}

void Screen::reset() {
    for (int i = 0; i < matrixSize; i++) {
        if (i % (width + 1) == width) {
            screenMatrix[i] = '\n';
            continue;
        }

        screenMatrix[i] = ' ';
    }
    screenMatrix[matrixSize - 1] = 0;
}

void Screen::input(char text, int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return;
    }

    screenMatrix[x + y * (width+1)] = text;
}

void Screen::rect(char text, int x, int y, int width, int height) {
    for (int i = x; i < x + width; i++) {
        for (int j = y; j < y + height; j++) {
            input(text, i, j);
        }
    }
}

void Screen::text(std::string text, int x, int y) {
    for (int i = 0, j = 0, k = 0; i < text.length(); i++, j++) {
        if (text[i] == '\n') {
            j = -1;
            k++;
            continue;
        }
        input(text[i], x + j, y + k);
    }
}

void Screen::print() {
    std::cout << "\x1b[?25l\x1b[1;1H";

    //printout += "\x1b[38;2;255;255;0m";

    std::cout << screenMatrix;
}