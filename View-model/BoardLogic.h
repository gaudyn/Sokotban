#ifndef BoardLogic_h
#define BoardLogic_h

#include <stdio.h>
#include <gtk/gtk.h>

#include "../Model/Board.h"

typedef struct pos Pos;

void initLogic(char *filePath);
void prepareBoard(Board b);
void revertBoard(void);

void movePlayer(int dy, int dx, GtkWidget *boardGrid);

gboolean isPuzzleSolved(void);
void puzzleSolved(void);

gboolean undoLastAction(void);
gboolean redoLastAction(void);

gboolean saveGameState(char *filePath);
gboolean loadGameState(char *filePath);

Board getCurrentBoard(void);
Board getMainBoard(void);
void updatePlayerPos(void);

#endif /* BoardLogic_h */
