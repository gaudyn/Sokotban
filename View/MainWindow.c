#include "MainWindow.h"
#include "../View-model/BoardOperations.h"

#include <gtk/gtk.h>

GtkWidget *mainWindow;

GtkWidget *initMainWindow(){
    mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(mainWindow), 500, 500);
    gtk_window_set_position(GTK_WINDOW(mainWindow), GTK_WIN_POS_CENTER);
    return mainWindow;
}

GtkWidget *getMainWindow(){
    return mainWindow;
}

//MARK: - Decoration
void setMainWindowTitle(char * title){
    gtk_window_set_title(GTK_WINDOW(mainWindow), title);
}

//MARK: - Dialogs
gint showEditionAlertDialog(){
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(mainWindow), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Plansza została edytowana.\nCzy chcesz ją zapisać?");
    gint res = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return res;
}


void showErrorDialog(const char * message){
    GtkWidget *warningDialog = gtk_message_dialog_new(GTK_WINDOW(mainWindow), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "%s", message);
    gtk_dialog_run(GTK_DIALOG(warningDialog));
    gtk_widget_destroy(warningDialog);
}
