#include "Board.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

struct board{
    char * filePath;

    int sizeX, sizeY;
    enum tile * tiles;

    int hiMoves, hiPushes;
};



//MARK: - Init/destroy
Board boardInit(int sizeX, int sizeY){
    Board b = malloc(sizeof(struct board));
    enum tile * tiles = malloc(sizeof(enum tile)*sizeX*sizeY);

    if(b == NULL || tiles == NULL) return NULL;

    for(int i = 0; i < sizeY*sizeX; i++) tiles[i] = 0;

    b->tiles = tiles;

    b->sizeX = sizeX;
    b->sizeY = sizeY;

    b->hiMoves = 999;
    b->hiPushes = 999;

    b->filePath = malloc(sizeof(char)*1000);
    *(b->filePath) = '\0';

    return b;
}

Board boardDestroy(Board b){
    free(b->tiles);
    free(b);
    return NULL;
}

//MARK: - Loading and saving
bool saveBoard(Board b, char * filePath){
    FILE* file = fopen(filePath, "w");
    if(file == NULL)
      return false;


    fprintf(file, "%s\n", b->filePath);
    fprintf(file, "%d %d\n", b->hiMoves, b->hiPushes);
    fprintf(file, "%d %d\n", b->sizeX, b->sizeY);

    for(int i = 0; i < b->sizeY; i++){
        for(int j = 0; j < b->sizeX; j++){
            fprintf(file, "%d ", getBoardValueAt(b, j, i));
        }
        fprintf(file, "\n");
    }

    if(fclose(file))
      return false;

    return true;
}

Board loadBoard(char * filePath){
    FILE * file = fopen(filePath, "r");
    if(file == NULL) return NULL;

    int moves, pushes;
    char newFilepath[1000] = {0};
    char c;
    int it = 0;

    while((c = getc(file)) != '\n')
      newFilepath[it++] = c;

    newFilepath[it] = '\0';

    fscanf(file, "%d %d",  &moves, &pushes);

    int sX, sY;
    fscanf(file, "%d %d", &sX, &sY);

    Board b = boardInit(sX, sY);

    if(b == NULL) return NULL;

    for(int i = 0; i < b->sizeY; i++){
        for(int j = 0; j < b->sizeX; j++){
            int T;
            fscanf(file, "%d", &T);
            setBoardValueAt(b, j, i, T);
        }
    }
    strcpy(b->filePath, strlen(newFilepath) != 0 ? newFilepath : filePath);
    b->hiMoves = moves;
    b->hiPushes = pushes;

    if(fclose(file)) return NULL;

    return b;
}

//MARK: - Getters and setters
int getBoardSizeX(Board b){
    return b->sizeX;
}
int getBoardSizeY(Board b){
    return b->sizeY;
}

void setBoardFilepath(Board b, char *filePath){
    strcpy(b->filePath, filePath);
}
char *getBoardFilepath(Board b){
    return b->filePath;
}

enum tile getBoardValueAt(Board b, int posX, int posY){
    return b->tiles[posY*(b->sizeX)+posX];
}
void setBoardValueAt(Board b, int posX, int posY, enum tile value){
    b->tiles[posY*(b->sizeX)+posX] = value;
}

char *getBoardFilename(Board b){
    if(strlen(b->filePath) == 0) return NULL;

    char *filename = malloc(50*sizeof(char));
    strcpy(filename, strrchr(b->filePath, '/')+1);
    *strchr(filename, '.') = '\0';
    return filename;
}

int getBoardHiMoves(Board b){
    return b->hiMoves;
}

void setBoardHiMoves(Board b, int value){
    b->hiMoves = value;
}

int getBoardHiPushes(Board b){
    return b->hiPushes;
}

void setBoardHiPushes(Board b, int value){
    b->hiPushes = value;
}

//MARK: - Helpers
int verifyBoard(Board b){
    int playerNum = 0, boxNum = 0, buttonNum = 0;
    int sizeX = getBoardSizeX(b), sizeY = getBoardSizeY(b);
    for(int i = 0; i < sizeY; i++){
        for(int j = 0; j < sizeX; j++){
            switch (getBoardValueAt(b, j, i)) {
                case PLAYER:
                    playerNum++;
                    break;
                case BOX:
                    boxNum++;
                    break;
                case BUTTON:
                    buttonNum++;
                    break;
                default:
                    break;
            }
        }
    }

    if(playerNum != 1) return PLAYER;

    if(boxNum > buttonNum) return BOX;
    if(buttonNum > boxNum) return BUTTON;

    return 0;
}

Board copyBoard(Board b){
    Board newB = boardInit(b->sizeX, b->sizeY);
    if(newB == NULL) return NULL;
    newB->filePath = b->filePath;
    newB->hiMoves = b->hiMoves;
    newB->hiPushes = b->hiPushes;

    for(int i = 0; i < b->sizeX*b->sizeY; i++){
        newB->tiles[i] = b->tiles[i];
    }
    return newB;
}

void printBoard(Board b){
    printf("\n\n\n");
    for(int y = 0; y < b->sizeY; y++){
        for(int x = 0; x < b->sizeX; x++){
            printf("%d ", getBoardValueAt(b, x, y));
        }
        printf("\n");
    }
}
