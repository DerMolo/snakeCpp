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

    int prevX; 
    int prevY; 

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
        //assuming the snake's initial orientation is pointed rightward 
        prevX = tailX;
        prevY = tailY - 1;
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

    cout << "\033[" << 2 << ";" << 1 << "H";
    int count = 0;
    int colTrack = colSize; 
    while(count<colSize*rowSize){
        if (count % colTrack == 0 && count != 0) {
            colTrack += colSize; 
            cout << endl;
        }
        if(world[count] == '#')
            cout << "\033[31m" << world[count] << "\033[0m";
        else if(world[count] != '.')
            cout << "\033[32m" << world[count] << "\033[0m";
        else
            cout << world[count];
        count++;
    } 
}

void debugWorld(char* world, Snake snakeTemp, Apple appTemp, int colSize, int rowSize) { //optimized 
    //"\033[31m" << "X" << "\033[0m" red character 

    //debugging
    cout << "\033[" << rowSize + 5 << ";" << 0 << "H" << "Previous Coord: "<<snakeTemp.prevX<<", "<<snakeTemp.prevY<<"  ";
    cout << "\033[" << rowSize + 6 << ";" << 0 << "H" << "Tail Coord: " << snakeTemp.tailX << ", " << snakeTemp.tailY<<"  ";
    cout << "\033[" << rowSize + 7 << ";" << 0 << "H" << "Head Coord: " << snakeTemp.headX << ", " << snakeTemp.headY<<"  ";

    //printing handled by updateWorld() 
}

void spawnSnake(char* world, Snake& tempSnake, scroll direction, int colSize) {

    int headInd = tempSnake.headX * colSize + tempSnake.headY;
    //converting 2d-index to 1-d index
    //world[headInd] = '@';
    world[headInd] = 'R';

    int nodeInd = tempSnake.tailX * colSize + tempSnake.tailY;

    while (nodeInd != headInd) { //snake will always start scrolling towards the right 
        //world[headInd] = '@';
        world[nodeInd] = 'R';
        nodeInd++; 
    }
}

void clearScreen(int rowSize, int colSize) {
    //clearing screen
    for (int i = 0; i < rowSize + 5; i++) {
        cout << "\033[" << i << ";0H" << string(colSize, ' ') << endl;
        this_thread::sleep_for(chrono::milliseconds(20));
    }
}

void bigPrint(string msg, int rowSize, int colSize,string color) {

    //color param must be an ANSI escape code: 
    /* 
    \033[31m = red 
    \033[33m = green 
    https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences
    */

    clearScreen(rowSize, colSize);

    for (int i = 0; i < msg.size(); i++) {
        string target = bigLetters[toupper(msg[i])];
        //printing each row of bigLetters[msg[i]]

        for (int x = 0; x < target.size(); x += 6)
            cout << "\033[" << (int)x / 6 + 1 << ";" << i * 6 << "H" << color << target.substr(x, 5) << "\033[0m";
    }
}

bool restart = false; 

void gameover(Snake& tempSnake, int rowSize, int colSize){
    bigPrint("gameover", rowSize, colSize,"\033[31m");
    this_thread::sleep_for(chrono::seconds(1)); 
    cout << "\033[10;0H" << "\t press 'R' to try again ";
    cout << "\033[11;0H" << "\033[33m" << "\t FINAL SCORE: " << tempSnake.bodyLength <<"\033[0m" << endl;
    int count = 5; 
    char userIn = ' ';
    while(count>0 && userIn != 'r') {
        if (_kbhit()) {
            userIn = _getch();
            if (userIn == 'r') {
                restart = true;
                break;
            }
        }
        cout << "\033[10;32H" << ": " << count-1;
        count--;
        this_thread::sleep_for(chrono::seconds(1));
    }
    if (!restart)
        exit(1);
}

void updateWorld(char* world, Snake& snakeTemp, scroll& direction, Apple*& appTemp, const int rowSize, const int colSize) {

    //renders the snake in the world according to the passed direction 
    //essentially draws a directionally-consistent line between the snake's head and tail

    char headBody = 'R';
    char tailBody = 'R';

    switch (direction)
    {
    case scroll::LEFT:
        headBody = 'L';
        snakeTemp.headY--;
        break;
    case scroll::RIGHT:
        headBody = 'R';
        snakeTemp.headY++;
        break;
    case scroll::UP:
        headBody = 'U';
        snakeTemp.headX--;
        break;
    case scroll::DOWN:
        headBody = 'D';
        snakeTemp.headX++;
        break;
    default:
        break;
    }

    int headInd = snakeTemp.headX * colSize + snakeTemp.headY;
    bool isSnakeBody = world[headInd] == 'L' || world[headInd] == 'R' || world[headInd] == 'U' || world[headInd] == 'D';

    if (isSnakeBody || world[headInd] == '#') {
        //printAll();
        gameover(snakeTemp, rowSize, colSize);
        return;
    }

    //trigerring tail growth 
    bool growSnake = false;
    if (world[headInd] == '+') {

        cout << "\033[" << appTemp->appX + 2 << ";" << appTemp->appY + 1 << "H" << "\033[32m" << headBody << "\033[0m";

        snakeTemp.bodyLength++;

        cout << "\033[" << rowSize + 4 << ";" << 0 << "H" <<  "\033[33m" << " SCORE: " << snakeTemp.bodyLength << "\033[0m";

        appTemp->exists = false;
        growSnake = true;
    }

    //converting 2d-index to 1-d index
    //world[headInd] = '@';
    world[headInd] = headBody;

    //printing head 
    cout << "\033[" << snakeTemp.headX + 2 << ";" << snakeTemp.headY + 1 << "H" << "\033[32m"<<world[headInd]<<"\033[0m";

    int tailInd = snakeTemp.tailX * colSize + snakeTemp.tailY;

    if (!growSnake) {

        world[tailInd] = '.';
        snakeTemp.prevX = tailInd/colSize;
        snakeTemp.prevY = tailInd % colSize;

        //clearing trail
        cout << "\033[" << snakeTemp.prevX + 2 << ";" << snakeTemp.prevY + 1 << "H" << '.';
         
        if (world[tailInd - 1] == 'L' && tailInd - 1 != headInd) {
            tailBody = 'L';
            tailInd--;
        }
        else if (world[tailInd + colSize] == 'D' && tailInd + colSize != headInd) {
            tailBody = 'D';
            tailInd += colSize;
        }
        else if (world[tailInd + 1] == 'R' && tailInd + 1 != headInd) {
            tailBody = 'R';
            tailInd++;
        }
        else if (world[tailInd - colSize] == 'U' && tailInd - colSize != headInd) {
            tailBody = 'U';
            tailInd -= colSize;
        }

        snakeTemp.tailX = tailInd / colSize;
        snakeTemp.tailY = tailInd % colSize;

        world[tailInd] = tailBody;

        //printing tail 
        cout << "\033[" << snakeTemp.tailX + 2 << ";" << snakeTemp.tailY + 1 << "H" << "\033[32m" << world[tailInd] << "\033[0m";
    }else 
        cout << "\033[" << snakeTemp.tailX + 2 << ";" << snakeTemp.tailY + 1 << "H" << "\033[33m" << world[tailInd] << "\033[0m";
}

int main() {
    const int width = 50;
    const int height = 20;

    int difficulty = 0; 

    bigPrint("snake", height, width,"\033[33m");
    cout << "\033[10;1H" << "Select a level of difficulty to begin:\n1) EASY\n2) MEDIUM\n3) HARD";
    
    char tempIn = ' ';
    bool x = false;
    do{
        if (_kbhit()) {
            tempIn = _getch();
            x = tempIn == '1' || tempIn == '2' || tempIn == '3';
        }
    }while (!x);

    clearScreen(height, width);
   
    switch (tempIn)
    {
    case '1':
        difficulty = 60;
        break;
    case '2':
        difficulty = 45;
    case '3':
        difficulty = 20;
        break;
    default:
        break;
    }

    do{
        if (restart) {
            restart = false;
            cout << "\033[0;0H" << string(100, ' ');
        }

        char world[height][width];

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (j == 0 || i == 0 || i == height - 1 || j == width - 1)
                    world[i][j] = '#';
                else
                    world[i][j] = '.';
            }
        }

        //SNAKE VARS 
        int bodyLength = 0;

        int headY = width / 2;
        int headX = height / 2;

        int tailY = headY - bodyLength;
        int tailX = headX;

        scroll direction = scroll::RIGHT;

        Snake mainSnake(bodyLength, headX, headY, tailX, tailY);
        Apple* manzana = new Apple();

        spawnSnake(*world, mainSnake, direction, width);
        printWorld(*world, width, height); //initial print 

        int keyPressed_debugIndex = 0;

        auto prevTime = chrono::system_clock::now();

        while (!restart) {
            //FPS CONTROL 
            this_thread::sleep_for(chrono::milliseconds(difficulty));

            auto now = chrono::system_clock::now();
            chrono::duration<float> elapsedTime = now - prevTime;

            cout << "\033[" << 1 << ";" << 0 << "H" << " FPS: " << 1.0 / elapsedTime.count();
            prevTime = chrono::system_clock::now();

            //spawning apple
            if (!manzana->exists) {
                do {
                    srand(time(NULL));
                    manzana->appX = rand() % (height - 2) + 1;
                    manzana->appY = rand() % (width - 2) + 2;
                } while (world[manzana->appX][manzana->appY] == 'L' || world[manzana->appX][manzana->appY] == 'R' || world[manzana->appX][manzana->appY] == 'U' || world[manzana->appX][manzana->appY] == 'D' || world[manzana->appX][manzana->appY] == '#');
                manzana->exists = true;
                world[manzana->appX][manzana->appY] = '+';

                //printing apple 
                cout << "\033[" << manzana->appX + 2 << ";" << manzana->appY + 1<< "H" << "\033[33m" << '+' << "\033[0m";
            }

            if (_kbhit()) {
                char userIn = _getch();

                if (userIn == 'w' && direction != scroll::DOWN)
                    direction = scroll::UP;
                else if (userIn == 's' && direction != scroll::UP)
                    direction = scroll::DOWN;
                else if (userIn == 'd' && direction != scroll::LEFT)
                    direction = scroll::RIGHT;
                else if (userIn == 'a' && direction != scroll::RIGHT)
                    direction = scroll::LEFT;

                //debugging 
                cout << "\033[" << height + 2 << ";" << 0 << "H" << " KEY PRESSED: ";
                cout << "\033[" << height + 2 << ";" << keyPressed_debugIndex + 17 << "H" << userIn;
                keyPressed_debugIndex = (keyPressed_debugIndex + 1) % 5;

                cout << "\033[" << height + 3 << ";" << 0 << "H" << " APPLE DATA: " << manzana->appX << ", " << manzana->appY << " EXISTS? " << manzana->exists;

                updateWorld(*world, mainSnake, direction, manzana, height, width);
                //debugWorld(*world, mainSnake, *manzana, width, height);
                continue;
            }
            //auto-scrolling for select direction
            updateWorld(*world, mainSnake, direction, manzana, height, width);
            //debugWorld(*world, mainSnake, *manzana, width, height);
        }
    } while (restart);
    return 0;
}
