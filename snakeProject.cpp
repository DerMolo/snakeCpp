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

enum class scroll {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

struct Snake {
    int bodyLength; 
    int headX; 
    int headY; 
    int tailX; 
    int tailY;
    Snake(int bLen, int hX, int hY, int tX, int tY) {
        bodyLength = bLen; 
        headX = hX;
        headY = hY; 
        tailX = tX; 
        tailY = tY; 
    }
};

void printWorld(char* world, int colSize, int rowSize) {
    //ANSI ESCAPE CODE: 
    // places cursors at some given position \033["row;colH
    cout << "\033[" << 2 << ";" << 0 << "H";
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


void updateSnake(char*& world, Snake tempSnake, scroll direction, int rowSize, int colSize) { 
    
    //renders the snake in the world according to the passed direction 
    //essentially draws a directionally-consistent line between the snake's head and tail 

    int temp; 
    int count = tempSnake.bodyLength; 
    bool dir = direction == scroll::LEFT || direction == scroll::RIGHT; 
    
    //converting 2d-index to 1-d index
    world[tempSnake.headX * colSize + tempSnake.headY] = '@';
    world[tempSnake.tailX * colSize + tempSnake.tailY] = '@';

    temp = tempSnake.headX * colSize + tempSnake.headY-1; 
    
    while (count > 0) {
        world[temp] = '@'; 
        temp -= dir ? 1 : colSize;  // conditionally traverses horizontally or vertically 
        count--; 
    }
}

int main() {

    const int width = 50; 
    const int height = 20; 

    char* world[height][width];

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            *world[i][j] = '.';
        }
    }

    //SNAKE VARS 
    int bodyLength = 2; 

    int headY = width/2; 
    int headX = height/2;

    int tailY;
    int tailX = headX-bodyLength; 

    Snake mainSnake(bodyLength, headX, headY, tailX, tailY); 

    updateSnake(**world, mainSnake, scroll::RIGHT, height, width);
    *world[headX][headY] = '@';

    printWorld((char*)world, width, height);
    this_thread::sleep_for(chrono::seconds(2));

    scroll direction = scroll::RIGHT;
    auto prevTime = chrono::system_clock::now();
    while (true) {

        this_thread::sleep_for(chrono::milliseconds(5));

        auto now = chrono::system_clock::now();
        chrono::duration<float> elapsedTime = now - prevTime; 
        cout << "\033[" << 1 << ";" << 0 << "H" << " FPS: " << 1.0 / elapsedTime.count();
        prevTime = chrono::system_clock::now(); 

        if (_kbhit()) {
            char userIn = _getch();
            *world[tailX][tailY] = '.';
            if (userIn == 'w') { //up
                headX--;
                direction = scroll::UP;
            }
            else if (userIn == 's') { //down
                headX++;
                direction = scroll::DOWN;
            }
            else if (userIn == 'd') { //right
                headY++;
                direction = scroll::RIGHT;
            }
            else if (userIn == 'a') { //left
                headY--;
                direction = scroll::LEFT;
            }
            
            //tail moves in the direction of main body. 
            //assumption: main body doesn't update, only tail and head 

            //if(world[tailX])
            //*world[tailX][tailY] = '@'; 
            //*world[headX][headY] = '@';

            printWorld((char*)world, width, height);
            continue; 
        }
        //auto-scrolling for select direction
        *world[headX][headY] = '.';
        switch (direction)
        {
        case scroll::LEFT:
            headY--;
            break;
        case scroll::RIGHT:
            headY++;
            break;
        case scroll::UP:
            headX--;
            break;
        case scroll::DOWN:
            headX++;
            break;
        default:
            break;
        }
        *world[headX][headY] = '@';
        printWorld((char*)world, width, height);
    }
    return 0;
}
