#ifndef MainGameToolbar_h
#define MainGameToolbar_h

#include <gtk/gtk.h>

GtkWidget *initMainToolbar(void);

void loadNewBoard(GtkWidget *widget, gpointer data);
void loadBoardState(GtkWidget *widget, gpointer data);
void saveBoardStateAs(GtkWidget *widget, gpointer data);

void undoBoardCallback(GtkWidget *widget, gpointer data);
void redoBoardCallback(GtkWidget *widget, gpointer data);
void resetBoard(GtkWidget *widget, gpointer data);
void updateEditButtonsState(gboolean undoState, gboolean redoState);

void changeMainGridSize(GtkWidget *widget, gpointer data);
#endif /* MainGameToolbar_h */
