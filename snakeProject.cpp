#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include <thread>
#include <chrono>
#include <iostream>
#include <vector>
#include <algorithm>
#include <conio.h>

void printWorld(char* world, int colSize, int rowSize) {
    //ANSI ESCAPE CODE: 
    // places cursors at some given position \033["row;colH
    cout << "\033[" << 1 << ";" << 0 << "H";
    int count = 0;
    int colTrack = colSize; 
    while(count<colSize*rowSize){
        if (count % colTrack == 0 && count != 0) {
            colTrack += colSize; 
            cout << endl;
        }
        cout << world[count];
        count++;
    } 
}

int main() {

    const int width = 50; 
    const int height = 20; 

    char world[height][width];

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            world[i][j] = '.';
        }
    }

    int snakeY = width/2; 
    int snakeX = height/2;

    world[snakeX][snakeY] = '*';
    printWorld((char*)world, width, height);
    this_thread::sleep_for(chrono::seconds(2));

    while (true) {
        if (_kbhit()) {
            char userIn = _kbhit();
            if (userIn == 'w')
                snakeX--;
            else if (userIn == 's')
                snakeX++;
            else if (userIn == 'd')
                snakeY++;
            else if (userIn == 'a')
                snakeY--;
        }
        else
            cout << "no input passed" << endl;
        world[snakeX][snakeY] = '*';
        printWorld((char*)world, width, height);
        cout << "\r";
    }
    return 0;
}
