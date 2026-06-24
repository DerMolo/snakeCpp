#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include <unordered_map>
#include <thread>
#include <chrono>
#include <algorithm>
#include <conio.h>

std::unordered_map<char, std::string> bigLetters = {
        {'A',
         "  -  \n"
         " / \\ \n"
         "|---|\n"
         "|   |\n"
         "|   |\n"},

        {'B',
         "|--\\ \n"
         "|  | \n"
         "|--/ \n"
         "|  | \n"
         "|--/ \n"},

        {'C',
         " /-- \n"
         "|    \n"
         "|    \n"
         "|    \n"
         " \\-- \n"},

        {'D',
         "|--\\ \n"
         "|   |\n"
         "|   |\n"
         "|   |\n"
         "|--/ \n"},

        {'E',
         "|--- \n"
         "|    \n"
         "|--  \n"
         "|    \n"
         "|--- \n"},

        {'F',
         "|--- \n"
         "|    \n"
         "|--  \n"
         "|    \n"
         "|    \n"},

        {'G',
         " /-- \n"
         "|    \n"
         "| --\\\n"
         "|   |\n"
         " \\--/\n"},

        {'H',
         "|   |\n"
         "|   |\n"
         "|---|\n"
         "|   |\n"
         "|   |\n"},

        {'I',
         "-----\n"
         "  |  \n"
         "  |  \n"
         "  |  \n"
         "-----\n"},

        {'J',
         "  ---\n"
         "    |\n"
         "    |\n"
         "|   |\n"
         " \\--/\n"},

        {'K',
         "|  / \n"
         "| /  \n"
         "|/   \n"
         "|\\   \n"
         "| \\  \n"},

        {'L',
         "|    \n"
         "|    \n"
         "|    \n"
         "|    \n"
         "|----\n"},

        {'M',
         "|\\ /|\n"
         "|/ \\|\n"
         "|   |\n"
         "|   |\n"
         "|   |\n"},

        {'N',
         "|   |\n"
         "|\\  |\n"
         "| \\ |\n"
         "|  \\|\n"
         "|   |\n"},

        {'O',
         " /-\\ \n"
         "|   |\n"
         "|   |\n"
         "|   |\n"
         " \\-/ \n"},

        {'P',
         "|--\\ \n"
         "|  | \n"
         "|--/ \n"
         "|    \n"
         "|    \n"},

        {'Q',
         " /-\\ \n"
         "|   |\n"
         "|   |\n"
         " \\-\\\n"
         "    \\\n"},

        {'R',
         "|--\\ \n"
         "|  | \n"
         "|--/ \n"
         "| \\  \n"
         "|  \\ \n"},

        {'S',
         " /-- \n"
         "|    \n"
         " \\--\\\n"
         "    |\n"
         "\\--/ \n"},

        {'T',
         "-----\n"
         "  |  \n"
         "  |  \n"
         "  |  \n"
         "  |  \n"},

        {'U',
         "|   |\n"
         "|   |\n"
         "|   |\n"
         "|   |\n"
         " \\-/ \n"},

        {'V',
         "|   |\n"
         "|   |\n"
         " \\ / \n"
         "  .  \n"
         "     \n"},

        {'W',
         "|   |\n"
         "|   |\n"
         "| | |\n"
         " \\|/ \n"
         "  -  \n"},

        {'X',
         "\\   /\n"
         " \\ / \n"
         "  |  \n"
         " / \\ \n"
         "/   \\\n"},

        {'Y',
         "\\   /\n"
         " \\ / \n"
         "  |  \n"
         "  |  \n"
         "  |  \n"},

        {'Z',
         "-----\n"
         "   / \n"
         "  /  \n"
         " /   \n"
         "-----\n"}
};

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

struct Apple {
    bool exists;
    int appX; 
    int appY; 
    Apple(bool here, int x, int y) {
        exists = here; 
        appX = x;
        appY = y;
    }
    Apple() {
        exists = false;
        appX = -1; 
        appY = -1; 
    }
};

void printWorld(char* world, int colSize, int rowSize) {
    //ANSI ESCAPE CODE: 
    // places cursors at some given position \033["row;colH

    //optimization: using an ansi escape code, only render the portion of the screen that's changed 
    //rather than rendering the entire world 

    //idea: rendering the radius of the snake (while scaling according to world size and snake size) 

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

    while (nodeInd != headInd) { //snake will always start scrolling towards the right 
        world[nodeInd] = '@';
        nodeInd++; 
    }
}

void bigPrint(string msg, int rowSize, int colSize) {
    //clearing screen
    for (int i = 0; i < rowSize + 5; i++) {
        cout << "\033[" << i << ";0H" << string(colSize, ' ') << endl;
        this_thread::sleep_for(chrono::milliseconds(20));
    }
    for (int i = 0; i < msg.size(); i++) {
        string target = bigLetters[toupper(msg[i])];
        //printing each row of bigLetters[msg[i]]

        for (int x = 0; x < target.size(); x += 6)
            cout << "\033[" << (int)x / 6 + 1 << ";" << i * 6 << "H" << target.substr(x, 5);
    }
}

void gameover(int rowSize, int colSize){
    bigPrint("gameover", rowSize, colSize);
    this_thread::sleep_for(chrono::seconds(30));
    exit(1);
}

void updateWorld(char* world, Snake& tempSnake, scroll& direction, Apple*& appTemp, const int rowSize, const int colSize) {

    //renders the snake in the world according to the passed direction 
    //essentially draws a directionally-consistent line between the snake's head and tail

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

    int headInd = tempSnake.headX * colSize + tempSnake.headY;
    if (world[headInd] == '@' || world[headInd] == '#') {
        gameover(rowSize, colSize);
        return; 
    }
    
    //trigerring tail growth 
    bool growSnake = false;
    if (world[headInd] == '+') {
        tempSnake.bodyLength++;
        appTemp->exists = false; 
        growSnake = true;
    }

    //converting 2d-index to 1-d index
    world[headInd] = '@';

    if (!growSnake) {
        int start = tempSnake.tailX * colSize + tempSnake.tailY;
        int nodeInd = start;

        world[nodeInd] = '.';

        //this fuckery will do
        //trailing tail according to neighbours and head's direction 

        //bad approach; causes game-breaking glitch 
        if (direction == scroll::RIGHT) { //ccw 
            if (world[nodeInd - 1] == '@' && nodeInd - 1 != headInd) {//left
                nodeInd--;
                //cout << "going left nodeInd: " << nodeInd << endl;
            }
            else if (world[nodeInd + colSize] == '@' && nodeInd + colSize != headInd) {//down
                nodeInd += colSize;
                //cout << "going down nodeInd: " << nodeInd << endl;
            }
            else if (world[nodeInd + 1] == '@' && nodeInd + 1 != headInd) {//right
                nodeInd++;
                //cout << "going right nodeInd: " << nodeInd << endl;
            }
            else if (world[nodeInd - colSize] == '@' && nodeInd - colSize != headInd) {//up
                nodeInd -= colSize;
                //cout << "going up nodeInd: "<<nodeInd<<endl;
            }
        }
        else if (direction == scroll::LEFT) { //ccw
            if (world[nodeInd + 1] == '@' && nodeInd + 1 != headInd) {//right
                nodeInd++;
                //cout << "going right nodeInd: " << nodeInd << endl;
            }
            else if (world[nodeInd - colSize] == '@' && nodeInd - colSize != headInd) {//up
                nodeInd -= colSize;
                //cout << "going up nodeInd: "<<nodeInd<<endl;
            }
            else if (world[nodeInd - 1] == '@' && nodeInd - 1 != headInd) {//left
                nodeInd--;
                //cout << "going left nodeInd: " << nodeInd << endl;
            }
            else if (world[nodeInd + colSize] == '@' && nodeInd + colSize != headInd) {//down
                nodeInd += colSize;
                //cout << "going down nodeInd: " << nodeInd << endl;
            }
        }
        else { //ccw
            if (world[nodeInd - colSize] == '@' && nodeInd - colSize != headInd) {//up
                nodeInd -= colSize;
                //cout << "going up nodeInd: "<<nodeInd<<endl;
            }
            else if (world[nodeInd + colSize] == '@' && nodeInd + colSize != headInd) {//down
                nodeInd += colSize;
                //cout << "going down nodeInd: " << nodeInd << endl;
            }
            else if (world[nodeInd - 1] == '@' && nodeInd - 1 != headInd) {//left
                nodeInd--;
                //cout << "going left nodeInd: " << nodeInd << endl;
            }
            else if (world[nodeInd + 1] == '@' && nodeInd + 1 != headInd) {//right
                nodeInd++;
                //cout << "going right nodeInd: " << nodeInd << endl;
            }
        }

        tempSnake.tailX = nodeInd / colSize;
        tempSnake.tailY = nodeInd % colSize;
        //cout << "tail: (" << tempSnake.tailX << ", " << tempSnake.tailY << ") " << endl;
        //cout << "head: (" << tempSnake.headX << ", " << tempSnake.headY << ") " << endl;

        world[nodeInd] = '@';
    }
}


int main() {
    const int width = 50; 
    const int height = 20; 

    char world[height][width];

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0 || i == 0 || i == height-1|| j == width-1)
                world[i][j] = '#';
            else 
                world[i][j] = '.';
        }
    }

    //SNAKE VARS 
    int bodyLength = 0; 

    int headY = width/2; 
    int headX = height/2;

    int tailY = headY-bodyLength;
    int tailX = headX; 

    scroll direction = scroll::RIGHT;

    Snake mainSnake(bodyLength, headX, headY, tailX, tailY); 
    Apple* manzana = new Apple();

    spawnSnake(*world, mainSnake, direction, width);
    printWorld(*world, width, height);

    int keyPressed_debugIndex = 0;

    auto prevTime = chrono::system_clock::now();

    while (true) {

        //FPS CONTROL 
        this_thread::sleep_for(chrono::milliseconds(20));

        auto now = chrono::system_clock::now();
        chrono::duration<float> elapsedTime = now - prevTime; 

        cout << "\033[" << 1 << ";" << 0 << "H" << " FPS: " << 1.0 / elapsedTime.count();
        prevTime = chrono::system_clock::now();
        //TODO: package game into a loop for optional restarting  

        //spawning apple
        if(!manzana->exists){
            do{
                srand(time(NULL));
                manzana->appX = (rand() % height - 1) + 2;
                manzana->appY = (rand() % width-1) + 2;
            } while (world[manzana->appX][manzana->appY] == '@' || world[manzana->appX][manzana->appY] == '#');
            manzana->exists = true;
            world[manzana->appX][manzana->appY] = '+';
        }

        if (_kbhit()) {
            char userIn = _getch();
            //refactoring to have updateSnake handle movement logic 
            if (userIn == 'w' && direction != scroll::DOWN)
                direction = scroll::UP;
            else if (userIn == 's' && direction != scroll::UP)
                direction = scroll::DOWN;
            else if (userIn == 'd' && direction != scroll::LEFT)
                direction = scroll::RIGHT;
            else if (userIn == 'a' && direction != scroll::RIGHT)
                direction = scroll::LEFT;

            //debugging 
            cout << "\033[" << height+2 << ";" << 0 << "H" << " KEY PRESSED: ";
            cout << "\033[" << height+2 << ";" << keyPressed_debugIndex+17 << "H" << userIn;
            keyPressed_debugIndex = (keyPressed_debugIndex + 1) % 5;

            cout << "\033[" << height + 3 << ";" << 0 << "H" << " APPLE DATA: "<<manzana->appX<<", "<<manzana->appY<< " EXISTS? "<<manzana->exists;

            cout << "\033[" << height + 4 << ";" << 0 << "H" << " SCORE: "<<mainSnake.bodyLength;



            updateWorld(*world, mainSnake, direction, manzana, height, width);
            printWorld(*world, width, height);
            continue; 
        }
        //auto-scrolling for select direction
        updateWorld(*world, mainSnake, direction, manzana, height, width);
        printWorld(*world, width, height);
    }
    return 0;
}
