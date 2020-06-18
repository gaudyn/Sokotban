#ifndef MainGameGrid_h
#define MainGameGrid_h

#include <stdio.h>
#include <gtk/gtk.h>

#include "../Model/Board.h"

GtkWidget *initBoardGrid(void);

gboolean mainBoardDraw(GtkWidget *widget, cairo_t *cr, gpointer data);

void loadBoardGrid(Board board);
void updateBoardGrid(Board board);
void updateGridTile(int posX, int posY, int value);

int setTileSize(int newSize, Board board);

void changeImageFile(GtkWidget * image, char * file);
void removeWidget(GtkWidget * widget, gpointer data);

#endif /* MainGameGrid_h */
