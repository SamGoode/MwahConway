#pragma once
#include <string>

class Screen {
    private:    
        char* screenMatrix;
        int matrixSize;
    
    public:
        int width;
        int height;

        Screen();

        Screen(int width, int height);

        ~Screen();

        Screen& operator=(const Screen& screen);

        void reset();

        void input(char text, int x, int y);

        void rect(char text, int x, int y, int width, int height);

        void text(std::string text, int x, int y);

        void print();
};