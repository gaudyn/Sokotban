#ifndef MainWindowMenu_h
#define MainWindowMenu_h

#include <gtk/gtk.h>
#include "../Model/Board.h"

GtkWidget * prepareMainWindowMenu(void);

//Create board
void showNewBoardDialog(GtkWidget * widget, gpointer data);
void closeDialogWindow(GtkWidget * widget, gpointer data);
void newBoardCallback(GtkWidget * widget, gpointer data);

//Save and load board
void showOpenBoardDialog(GtkWidget *widget, gpointer data);
void saveBoardWithDialog(GtkWidget *widget, gpointer data);
void saveBoardWithoutDialog(GtkWidget *widget, gpointer data);

void saveBoardCallback(GtkWidget *widget, gpointer data);
GtkFileChooserConfirmation confirmOverwriteCallback(GtkFileChooser *chooser, gpointer data);

void setSavesEnabled(gboolean sensitivity);

void showInvalidBoardDialog(int code, void (*saveFunc)(GtkWidget*, gpointer));

//View
void setGridScale(GtkWidget * widget, gpointer data);

//Edit
void undoButtonCallback(GtkWidget *widget, gpointer data);
void redoButtonCallback(GtkWidget *widget, gpointer data);

void updateEditButtons(gboolean undoState, gboolean redoState);

#endif /* MainWindowMenu_h */
