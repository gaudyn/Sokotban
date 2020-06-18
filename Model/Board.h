#ifndef Board_h
#define Board_h

#include <stdio.h>
#include <stdbool.h>

typedef struct board *Board;

enum tile{
    EMPTY, WALK, BRICK, PLAYER, BOX, BUTTON
};

Board boardInit(int sizeX, int sizeY);
Board boardDestroy(Board b);

bool saveBoard(Board b, char *filePath);
Board loadBoard(char * filePath);


int getBoardSizeX(Board b);
int getBoardSizeY(Board b);

void setBoardFilepath(Board b, char *filePath);
char *getBoardFilepath(Board b);

enum tile getBoardValueAt(Board b, int posX, int posY);
void setBoardValueAt(Board b, int posX, int posY, enum tile value);

char *getBoardFilename(Board b);

int getBoardHiMoves(Board b);
void setBoardHiMoves(Board b, int value);
int getBoardHiPushes(Board b);
void setBoardHiPushes(Board b, int value);

int verifyBoard(Board b);
Board copyBoard(Board b);
void printBoard(Board b);
#endif /* Board_h */
