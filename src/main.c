// © MareksCode 2024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

const bool SUCCESS = true;
const bool FAILURE = false;

const int COMPUTER_PLAYER_NUM = 2;
const char CORRESPONDING_CHARACTERS_TO_BOARD_NUMBERS[4] = {' ','X','O','/'};
const int BOARD_SIZE = 3; //wenn man das ändert, muss man den DISALLOWED_POSITIONS_FOR_XY auch ändern!

int returnCorrespondingNumber(char num) {
    if (num == 'A') {
        return 0;
    }
    if (num == 'B') {
        return 1;
    }
    return 2;
}

void displayBoard(int arrayToPrint[BOARD_SIZE][BOARD_SIZE]) {
    printf("\n\n     A    B    C\n  #----#----#----#\n"); //menschenfreundlichkeit..

    for (int a = 0; a < BOARD_SIZE; a++) {
        printf("%i | ", a); //menschenfreundlichkeit..

        for (int b = 0; b < BOARD_SIZE; b++) {
            int currentValue = arrayToPrint[b][a];

            printf(" %c | ", CORRESPONDING_CHARACTERS_TO_BOARD_NUMBERS[currentValue]);
        }
        printf("\n  #----#----#----#\n");
    }
    printf("\n\n");
}

int getLimit(int num) {
    if (num < 0) {
        return BOARD_SIZE + num;
    }
    if (num >= BOARD_SIZE) {
        return num-BOARD_SIZE;
    }
    return num;
}

bool evaluatePlacement(int board[BOARD_SIZE][BOARD_SIZE], int currentPlayerNumber, int columnNum, int row) {
    int disallowed[3][3] = {{0,1,0},{1,0,1},{0,1,0}};
    int xyAllowed[3][3] = {{0,0,1},{0,1,0},{1,0,0}};
    int yxAllowed[3][3] = {{1,0,0},{0,1,0},{0,0,1}};

    bool x = true;
    bool y = true;
    bool xy = true;
    bool yx = true;
    for (int a = 0; a < BOARD_SIZE; a += 1) {
        if (board[getLimit(columnNum + a)][row] != currentPlayerNumber) {
            x = false;
            //if ((a!=BOARD_SIZE-1)&&(getLimit(columnNum + a) == columnNum)) {x = true;}
        }
        if (board[columnNum][getLimit(row + a)] != currentPlayerNumber) {
            y = false;
            //if ((a!=BOARD_SIZE-1)&&(getLimit(row + a) == row)) {y = true;}
        }
        if (disallowed[columnNum][row] == 0) { //wenn erlaubt
            if (yxAllowed[columnNum][row] == 1) {
                if (board[getLimit(columnNum + a)][getLimit(row + a)] != currentPlayerNumber) {
                    xy = false;
                    //if ((a!=BOARD_SIZE-1)&&(getLimit(row + a) == row)&&(getLimit(columnNum + a) == columnNum)) {xy = true;}
                }
            } else {
                xy = false;
            }
            if (xyAllowed[columnNum][row] == 1) {
                if (board[getLimit(columnNum + a)][getLimit(row - a)] != currentPlayerNumber) {
                    yx = false;
                    //if ((a!=BOARD_SIZE-1)&&(getLimit(row - a) == row)&&(getLimit(columnNum + a) == columnNum)) {y = true;}
                }
            } else {
                yx = false;
            }
        } else {
            xy = false;
            yx = false;
        }
    }
    //printf("%d\n", x);
    //printf("%d\n", y);
    //printf("%d\n", xy);
    //printf("%d\n", yx);

    if (x) {
        for (int a = 0; a <= BOARD_SIZE; a += 1) {
            board[getLimit(columnNum + a)][row] = 3;
        }
    }
    if (y) {
        for (int a = 0; a <= BOARD_SIZE; a += 1) {
            board[columnNum][getLimit(row + a)] = 3;
        }
    }
    if (xy) {
        for (int a = 0; a <= BOARD_SIZE; a += 1) {
            board[getLimit(columnNum + a)][getLimit(row + a)] = 3;
        }
    }
    if (yx) {
        for (int a = 0; a <= BOARD_SIZE; a += 1) {
            board[getLimit(columnNum + a)][getLimit(row - a)] = 3;
        }
    }

    return x || y || xy || yx;
}

bool checkIfDraw(int board[BOARD_SIZE][BOARD_SIZE]) {
    bool freeSpace = false;

    for (int c = 1; c <=2; c+=1) {//alle spieler
        int playerBoard[BOARD_SIZE][BOARD_SIZE];
        for (int a = 0; a < BOARD_SIZE; a++) {
            for (int b = 0; b < BOARD_SIZE; b++) {
                playerBoard[a][b] = board[a][b];
                if (board[a][b] == 0) {
                    freeSpace = true;
                    playerBoard[a][b] = c;
                }
            }
        }
        if (!freeSpace) {
            return SUCCESS;
        }
        for (int a = 0; a < BOARD_SIZE; a += 1) {//höhe //das ist eigentlich unnötig
            for (int b = 0; b < BOARD_SIZE; b += 1) {//breite
                if (playerBoard[a][b] == c) {
                    if (evaluatePlacement(playerBoard, c, a, b)) {
                        return FAILURE;
                    }
                }
            }
        }
    }

    return SUCCESS;
}

bool evaluatePlacementOLD(int board[BOARD_SIZE][BOARD_SIZE], int currentPlayerNumber, int columnNum, int row) {
    int middle = board[columnNum][row];
    //printf("middle:%i\n", middle);
    int north = board[getLimit(columnNum+1)][row];
   // printf("north:%i\n", north);
    int east = board[columnNum][getLimit(row+1)];
    //printf("east:%i\n", east);
    int south = board[getLimit(columnNum-1)][row];
    //printf("south:%i\n", south);
    int west = board[columnNum][getLimit(row-1)];
    //printf("west:%i\n", west);
    int northwest = board[getLimit(columnNum+1)][getLimit(row-1)];
    //printf("northwest:%i\n", northwest);
    int northeast = board[getLimit(columnNum+1)][getLimit(row+1)];
    printf("northeast:%i\n", northeast);
    int southeast = board[getLimit(columnNum-1)][getLimit(row+1)];
    //printf("southeast:%i\n", southeast);
    int southwest = board[getLimit(columnNum-1)][getLimit(row-1)];
    printf("southwest:%i\n", southwest);

    bool vertical = north == currentPlayerNumber && south == currentPlayerNumber; //mitte ist nicht nötig, weil sie von der funk vorrausgesetzt wird
    bool horizontal = east == currentPlayerNumber && west == currentPlayerNumber;
    bool oblique1 = northwest == currentPlayerNumber && southeast == currentPlayerNumber;
    bool oblique2 = northeast == currentPlayerNumber && southwest == currentPlayerNumber;

    if (vertical) {
        printf("vertical win\n");
        return SUCCESS;
    }
    if (horizontal) {
        printf("horizontal win\n");
        return SUCCESS;
    }
    if (oblique1) {
        printf("oblique1 win\n");
        return SUCCESS;
    }
    if (oblique2) {
        printf("oblique2 win\n");
        return SUCCESS;
    }

    return FAILURE;
}

int checkArguments(int row, int column) {
    if (row >= 0 && row < BOARD_SIZE) {
        if (column >= 0 && column < BOARD_SIZE) {
            return 1;
        }
    }
    return 0;
}

bool checkIf1Player() {
    printf("Input a 0 if you want to play against the computer. 1 if you want the 2 player mode\n");
    int num = 0;
    scanf("%i%*c", &num);

    return num;
}

bool checkForBotPlacement(int board[BOARD_SIZE][BOARD_SIZE]) {
    const int a = rand()%3;
    const int b = rand()%3;

    if (board[a][b] == 0) {
        board[a][b] = COMPUTER_PLAYER_NUM;

        return evaluatePlacement(board, COMPUTER_PLAYER_NUM, a, b);
    } else {
        return checkForBotPlacement(board);
    }
}

bool checkForPlacement(int board[BOARD_SIZE][BOARD_SIZE], int currentPlayerNumber) {
    int row;
    char column;

    if (scanf("%c%i%*c", &column, &row) == 2) {//mega der scheiß; ToDo: replacen
        const int columnNum = returnCorrespondingNumber(column);
       // printf("calc: %i%i\n", columnNum, row);

        if (checkArguments(row, columnNum) == 0) {
            printf("wrong input! Please try typing in the following format:\ncolumn+row -> for ex. A1 or C2\n");
            checkForPlacement(board, currentPlayerNumber);
        } else if (board[columnNum][row] != 0) {
            printf("wrong input! There is already something at that position\n");
            checkForPlacement(board, currentPlayerNumber);
        } else {
            printf("success!\n");
            board[columnNum][row] = currentPlayerNumber;

            return evaluatePlacement(board, currentPlayerNumber, columnNum, row);
        }
    } else {
        printf("wrong input!\n");
        checkForPlacement(board, currentPlayerNumber);
    }

    return FAILURE; //Player hat nicht gewonnen
}

bool main(void) {
    srand(time(NULL)); //seed für rnd setzen

    int board[BOARD_SIZE][BOARD_SIZE]; //Möglichkeiten: 0: nix, 1: Spieler1, 2: Spieler2
    for (int a = 0; a < BOARD_SIZE; a++) {
        for (int b = 0; b < BOARD_SIZE; b++) {
            board[a][b] = 0; //Auf default werte setzen
        }
    }

    int playerMode = checkIf1Player();

    int currentPlayer = 0;
    displayBoard(board);
    bool gameRunning = true;

    while (gameRunning) {
        currentPlayer = currentPlayer+1;
        if (currentPlayer == 3) {
            currentPlayer = 1;
        }

        if (playerMode == 0 && currentPlayer == COMPUTER_PLAYER_NUM) {
            gameRunning = !checkForBotPlacement(board);
        } else {
            printf("[%c] Player %i is on turn! Please input:\n", CORRESPONDING_CHARACTERS_TO_BOARD_NUMBERS[currentPlayer], currentPlayer);
            gameRunning = !checkForPlacement(board, currentPlayer);
        }

        displayBoard(board);

        bool draw = checkIfDraw(board);
        if (draw && gameRunning) { //gamerunning, weil sonst jeder vertikale win als draw zählt
            currentPlayer = 0;
            gameRunning = false;
        }
    }

    if (currentPlayer>0) {
        printf("\n\nPlayer %i [%c] won the game!", currentPlayer, CORRESPONDING_CHARACTERS_TO_BOARD_NUMBERS[currentPlayer]);
    } else {
        printf("\n\nThe game resulted in a draw!");
    }

    return SUCCESS;
}