#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include <unordered_map>
#include <thread>
#include <chrono>
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

void spawnSnake(char* world, Snake& tempSnake, scroll direction, int colSize) {

    int headInd = tempSnake.headX * colSize + tempSnake.headY;
    //converting 2d-index to 1-d index
    world[headInd] = '@';

    int nodeInd = tempSnake.tailX * colSize + tempSnake.tailY;

    while (nodeInd != headInd) { //snake will always start scroll towards the right 
        world[nodeInd] = '@';
        nodeInd++; 
    }
}

void updateWorld(char* world, Snake& tempSnake, scroll& direction, int colSize) {

    //renders the snake in the world according to the passed direction 
    //essentially draws a directionally-consistent line between the snake's head and tail
    // movement conditions: 
    /* 
    I initially assumed the tail and head are the only moving parts 

    - each node must move towards the direction of the neighboring node.
    - this per-node search is terminated when: node.coord == head.coord
    */

    switch (direction)
    {
    case scroll::LEFT:
        tempSnake.headY--;
        break;
    case scroll::RIGHT:
        tempSnake.headY++;
        break;
    case scroll::UP:
        tempSnake.headX--;
        break;
    case scroll::DOWN:
        tempSnake.headX++;
        break;
    default:
        break;
    }

    unordered_map<int, bool> visitedNodes; 
    //possible optimization: only the back 2 nodes need to move (along with the head) 

    int headInd = tempSnake.headX * colSize + tempSnake.headY;
    //converting 2d-index to 1-d index
    world[headInd] = '@';

    int start = tempSnake.tailX * colSize + tempSnake.tailY;
    int nodeInd = start; 

    bool updateTail = false; 
    while (nodeInd != headInd) {
        //finding neighbours
        if (nodeInd == start) {
            world[nodeInd] = '.';
            updateTail = true; 
        }
        if (world[nodeInd - colSize] == '@' && visitedNodes[nodeInd-colSize] != true) {//up
            nodeInd -= colSize;
            //cout << "going up nodeInd: "<<nodeInd<<endl;
        }
        else if (world[nodeInd + colSize] == '@' && visitedNodes[nodeInd + colSize] != true) {//down
            nodeInd += colSize;
            //cout << "going down nodeInd: " << nodeInd << endl;
        }
        else if (world[nodeInd - 1] == '@' && visitedNodes[nodeInd - 1] != true) {//left
            nodeInd--;
            //cout << "going left nodeInd: " << nodeInd << endl;
        }
        else if (world[nodeInd + 1] == '@' && visitedNodes[nodeInd + 1] != true) {//right
            nodeInd++;
            //cout << "going right nodeInd: " << nodeInd << endl;
        }
        if (updateTail) {
            tempSnake.tailX = nodeInd / colSize;
            tempSnake.tailY = nodeInd % colSize; 
            //cout << "tail: (" << tempSnake.tailX << ", " << tempSnake.tailY << ") " << endl;
            //cout << "head: (" << tempSnake.headX << ", " << tempSnake.headY << ") " << endl;

            updateTail = false; 
        }
        world[nodeInd] = '@';
        visitedNodes[nodeInd] = true;
    }
    //cout << "loop complete\n";
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

    //SNAKE VARS 
    int bodyLength = 4; 

    int headY = width/2; 
    int headX = height/2;

    int tailY = headY-bodyLength;
    int tailX = headX; 

    scroll direction = scroll::RIGHT;

    Snake mainSnake(bodyLength, headX, headY, tailX, tailY); 

    cout << "TAILX: " << mainSnake.tailX << endl;
    cout << "TAILY: " << mainSnake.tailY << endl;
    cout << "BODY LENGTH: " << mainSnake.bodyLength << endl;

    //this_thread::sleep_for(chrono::seconds(3));

    spawnSnake(*world, mainSnake, direction, width);
    printWorld(*world, width, height);

    //this_thread::sleep_for(chrono::seconds(2));

    auto prevTime = chrono::system_clock::now();
    while (true) {

        //FPS CONTROL 
        this_thread::sleep_for(chrono::milliseconds(5));

        auto now = chrono::system_clock::now();
        chrono::duration<float> elapsedTime = now - prevTime; 
        cout << "\033[" << 1 << ";" << 0 << "H" << " FPS: " << 1.0 / elapsedTime.count();
        prevTime = chrono::system_clock::now(); 

        if (_kbhit()) {
            char userIn = _getch();
            //world[mainSnake.tailX][mainSnake.tailY] = '.';
            //refactoring to have updateSnake handle movement logic 
            if (userIn == 'w') { //up
                //mainSnake.headX--;
                direction = scroll::UP;
            }
            else if (userIn == 's') { //down
                //mainSnake.headX++;
                direction = scroll::DOWN;
            }
            else if (userIn == 'd') { //right
                //mainSnake.headY++;
                direction = scroll::RIGHT;
            }
            else if (userIn == 'a') { //left
                //mainSnake.headY--;
                direction = scroll::LEFT;
            }
            
            updateWorld(*world, mainSnake, direction, width);
            printWorld(*world, width, height);
            continue; 
        }
        //auto-scrolling for select direction
        //world[mainSnake.tailX][mainSnake.tailY] = '.'; //clearing snake trail
        
        //cout << "called scroll function: " << endl;
        updateWorld(*world, mainSnake, direction, width);
        //cout << "completed scroll function: " << endl;

        printWorld(*world, width, height);
    }
    return 0;
}
