#include <stdio.h>
#include <gtk/gtk.h>

#include "Model/Board.h"
#include "View-model/BoardLogic.h"

#include "View/MainWindow.h"
#include "View/GameToolbar.h"
#include "View/GameGrid.h"

gboolean keypressCallback(GtkWidget *widget, GdkEventKey *event, GtkWidget *mainBoardGrid);

int main(int argc, char * argv[]) {

    gtk_init(&argc, &argv);

    //MARK: - Main window
    GtkWidget *mainWindow = initMainWindow();
    setMainWindowTitle("SoKOTban");
    gtk_window_set_icon(GTK_WINDOW(mainWindow), gdk_pixbuf_new_from_file("Tiles/player_button.png", NULL));
    g_signal_connect(G_OBJECT(mainWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);


    //MARK: - Main toolbar
    GtkWidget *mainToolbar = initMainToolbar();

    //MARK: - Main board grid
    GtkWidget *mainBoardContainer = initBoardGrid();
    GtkWidget *mainGrid = (GtkWidget *)gtk_container_get_children(GTK_CONTAINER(mainBoardContainer))->data;

    gtk_widget_add_events(mainWindow, GDK_KEY_PRESS_MASK);
    g_signal_connect(G_OBJECT(mainWindow), "key_press_event", G_CALLBACK(keypressCallback), mainGrid);

    //MARK: - Main container
    GtkWidget *mainContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(mainWindow), mainContainer);

    gtk_box_pack_start(GTK_BOX(mainContainer), mainToolbar, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mainContainer), mainBoardContainer, TRUE, TRUE, 0);


    gtk_widget_show_all(mainWindow);

    //MARK: - Choose dialog
    GtkWidget *firstDialog = gtk_message_dialog_new(GTK_WINDOW(getMainWindow()), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_QUESTION, GTK_BUTTONS_NONE, "Co chcesz zrobić?");
    gtk_dialog_add_buttons(GTK_DIALOG(firstDialog), "Wczytaj zapis", GTK_RESPONSE_YES, "Wczytaj nową planszę", GTK_RESPONSE_ACCEPT, NULL);
    int res = gtk_dialog_run(GTK_DIALOG(firstDialog));
    gtk_widget_destroy(firstDialog);

    if(res == GTK_RESPONSE_YES) loadBoardState(NULL, NULL);
    else if(res == GTK_RESPONSE_ACCEPT) loadNewBoard(NULL, NULL);


    gtk_main();

    return 0;
}

//MARK: - Player movement
gboolean keypressCallback(GtkWidget *widget, GdkEventKey *event, GtkWidget *mainBoardGrid){
    if(event->state) return FALSE;
    if(event->keyval == GDK_KEY_Up || event->keyval == GDK_KEY_w){
        movePlayer(0, -1, mainBoardGrid);
        return TRUE;
    }
    if(event->keyval == GDK_KEY_Down || event->keyval == GDK_KEY_s){
        movePlayer(0, 1, mainBoardGrid);
        return TRUE;
    }
    if(event->keyval == GDK_KEY_Left || event->keyval == GDK_KEY_a){
        movePlayer(-1, 0, mainBoardGrid);
        return TRUE;
    }
    if(event->keyval == GDK_KEY_Right || event->keyval == GDK_KEY_d){
        movePlayer(1, 0, mainBoardGrid);
        return TRUE;
    }

    return FALSE;
}
