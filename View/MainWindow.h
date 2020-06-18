#ifndef MainWindow_h
#define MainWindow_h

#include <gtk/gtk.h>

GtkWidget *initMainWindow(void);
GtkWidget *getMainWindow(void);

//Decoration
void setMainWindowTitle(char * title);

//Dialog
gint showEditionAlertDialog(void);
void showErrorDialog(const char * message);

#endif /* MainWindow_h */
