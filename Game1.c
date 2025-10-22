#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <stdbool.h>

#define mapXlen 60
#define mapYlen 20

#define SnakeLen 60*20
#define FPS_Game 15

// setup map
char map[mapYlen][mapXlen];
char key;
int redX, redY;
char redOut = '$';
int Score = 1;
bool clock_FPS = 0;
bool T1 = 0;
float Score_FPS = 0;

char SnakeOut[2] = {'#', '&'};
int SnakePoint[SnakeLen][3];

int rows = sizeof(SnakePoint) / sizeof(SnakePoint[0]);
int cols = sizeof(SnakePoint[0]) / sizeof(SnakePoint[0][0]); // 3

// FPS
int FPS;

void PrintCharColor(char c, int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    int oldColor = csbi.wAttributes;
    SetConsoleTextAttribute(hConsole, color);

    printf("%c", c);

    SetConsoleTextAttribute(hConsole, oldColor);
}

int random_between(int min, int max) {
    return min + rand() % (max - min + 1);
}

void random_red() {
    while(1) {
        redX = random_between(0, mapXlen - 1);
        redY = random_between(0, mapYlen - 1);

        T1 = 0;
        for(int i = 0; i < Score; i++) {
            if(SnakePoint[i][0] == redX && SnakePoint[i][1]== redY) {
                T1 = 1;
            }
        }
        if(!T1) {
            return ;
        }
    }
}

void Snake_map() {
    if(SnakePoint[0][0] == redX && SnakePoint[0][1] == redY) {
        Score++;
        Score_FPS = Score / 5;
        random_red();
    }

    for (int i = Score - 1; i > 0; i--) {
        SnakePoint[i][0] = SnakePoint[i - 1][0]; // X
        SnakePoint[i][1] = SnakePoint[i - 1][1]; // Y
        SnakePoint[i][2] = SnakePoint[i - 1][2]; // Direction
    }

    if (SnakePoint[0][2] == 1) SnakePoint[0][1]--;     // UP
    if (SnakePoint[0][2] == 2) SnakePoint[0][0]++;     // RIGHT
    if (SnakePoint[0][2] == 3) SnakePoint[0][1]++;     // DOWN
    if (SnakePoint[0][2] == 4) SnakePoint[0][0]--;     // LEFT

    // X
    if(SnakePoint[0][0] == -1) { SnakePoint[0][0] += mapXlen; }
    else if(SnakePoint[0][0] == mapXlen) { SnakePoint[0][0] -= mapXlen; }
    // Y
    if(SnakePoint[0][1] == -1) { SnakePoint[0][1] += mapYlen; }
    else if(SnakePoint[0][1] == mapYlen) { SnakePoint[0][1] -= mapYlen; }

    if(Score > 1) {
        for(int i = 1; i < Score; i++) {
            if(SnakePoint[0][0] == SnakePoint[i][0] && SnakePoint[0][1] == SnakePoint[i][1]) {
                Score = 1;
            }
        }
    }
}

void Start_map() {
    printf("|");
    for(int i = 0; i < (int)((mapXlen - 12)/2); i++) {
        printf("-");
    }
    printf(" Snake Game ");
    for(int i = 0; i < (int)((mapXlen - 12)/2); i++) {
        printf("-");
    }
    printf("|\n");
}

void showMap() {
    for(int mapY = 0; mapY < mapYlen; mapY++) {
        printf("|");
        for(int mapX = 0; mapX < mapXlen; mapX++) {
            if(map[mapY][mapX] == '#') {
                //system("color a");
                PrintCharColor(map[mapY][mapX], 10);
            }else if(map[mapY][mapX] == '&') {
                PrintCharColor(map[mapY][mapX], 9);
            }else if(map[mapY][mapX] == '$') {
                PrintCharColor(map[mapY][mapX], 12);
            }else {
                PrintCharColor(map[mapY][mapX], 7);
            }
            // printf("%c", map[mapY][mapX]);
        }
        printf("|\n");
    }
    printf("|");
    for(int i = 0; i < mapXlen; i++) {
        printf("-");
    }
    printf("|\n");
}

void Create_Snake() {
    for(int i = 0; i < Score; i++){
        map[SnakePoint[i][1]][SnakePoint[i][0]] = i == 0 ? SnakeOut[1] : SnakeOut[0];
    }
}

// no Snake
void clear_map() {
    for (int y = 0; y < mapYlen; y++) {
        for (int x = 0; x < mapXlen; x++) {
            map[y][x] = ' ';
        }
    }
    map[redY][redX] = redOut;
}

int main() {
    // setup
    system("cls");

    printf("setup Game...");
    srand(time(NULL));

    // setup map
    for (int y = 0; y < mapYlen; y++) {
        for (int x = 0; x < mapXlen; x++) {
            map[y][x] = ' ';
        }
    }

    // Setup Snake
    redX = random_between(0, mapXlen - 1);
    redY = random_between(0, mapYlen - 1);

    clear_map();

    while(1) {
        // loop
        printf("\033[2J");
        printf("\033[H");
        
        Start_map();
        Snake_map();
        clear_map();
        Create_Snake();

        showMap();
        
        printf("\n");
        printf("Score : %d\n", Score - 1);

        if (_kbhit()) {
            key = _getch();
            // printf("button is : %c\n", key); // test Button

            if(key == 'w') {
                if(SnakePoint[0][2] != 3) {
                    SnakePoint[0][2] = 1;
                }
            }else if(key == 'd') {
                if(SnakePoint[0][2] != 4) {
                    SnakePoint[0][2] = 2;
                }
            }else if(key == 's') {
                if(SnakePoint[0][2] != 1) {
                    SnakePoint[0][2] = 3;
                }
            }else if(key == 'a') {
                if(SnakePoint[0][2] != 2) {
                    SnakePoint[0][2] = 4;
                }
            }

            if (key == 'q') {
                printf("Exit");
                break;
            }
        }

        // FPS
        FPS = 1000 / (FPS_Game + Score_FPS);
        clock_FPS = !clock_FPS;
        Sleep(FPS); // delay(FPS)
    }

    return 0;
}