#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

#include "View/EditorMenu.h"
#include "Model/Board.h"
#include "View-model/BoardOperations.h"
#include "View/MainWindow.h"
#include "View/EditorToolbar.h"

GtkWidget *mainGrid;
Board board;


void addActionCallback(GtkWidget *widget, gpointer data);
gboolean shouldCloseWindow(GtkWidget *widget, GdkEvent *event, gpointer data);

int main(int argc, char *argv[]){

    gtk_init(&argc, &argv);

    //MARK: Window
    GtkWidget *mainWindow = initMainWindow();
    setMainWindowTitle("SoKOTban Edytor - Pusty");
    gtk_window_set_icon(GTK_WINDOW(mainWindow), gdk_pixbuf_new_from_file("Tiles/player-walk.png", NULL));
    g_signal_connect(G_OBJECT(mainWindow), "delete-event", G_CALLBACK(shouldCloseWindow), NULL);

    //MARK: Main menu
    GtkWidget * mainMenu = prepareMainWindowMenu();

    //MARK: Tiles toolbar
    GtkWidget * tileToolbar = initToolbar();

    //MARK: Main board editor
    GtkWidget *emptyBoardLabel = gtk_label_new("Aby rozpocząć edycję planszy utwórz lub otwórz plik");
    gtk_widget_set_sensitive(emptyBoardLabel, FALSE);

    GtkWidget *scrollableArea = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollableArea), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    GtkWidget *scrollAdapter = gtk_viewport_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrollableArea), scrollAdapter);
    gtk_widget_set_vexpand(scrollAdapter, TRUE);
    gtk_widget_set_hexpand(scrollAdapter, TRUE);

    GtkWidget *gridEventBox = gtk_event_box_new();
    gtk_widget_set_sensitive(gridEventBox, FALSE);
    gtk_container_add(GTK_CONTAINER(scrollAdapter), gridEventBox);

    gtk_widget_set_halign(gridEventBox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(gridEventBox, GTK_ALIGN_CENTER);
    gtk_widget_set_vexpand(gridEventBox, TRUE);
    gtk_widget_set_hexpand(gridEventBox, TRUE);

    mainGrid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(mainGrid), 1);
    gtk_grid_set_row_spacing(GTK_GRID(mainGrid), 1);
    gtk_grid_set_column_homogeneous(GTK_GRID(mainGrid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(mainGrid), TRUE);
    gtk_widget_set_halign(mainGrid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(mainGrid, GTK_ALIGN_CENTER);
    gtk_widget_set_vexpand(mainGrid, TRUE);
    gtk_widget_set_hexpand(mainGrid, TRUE);



    gtk_grid_attach(GTK_GRID(mainGrid), emptyBoardLabel, 0, 0, 1,1);

    gtk_widget_set_events(mainGrid, GDK_POINTER_MOTION_MASK|GDK_BUTTON_RELEASE_MASK);
    gtk_container_add(GTK_CONTAINER(gridEventBox), mainGrid);

    g_signal_connect(G_OBJECT(gridEventBox), "button-press-event", G_CALLBACK(fillBoard), NULL);
    g_signal_connect(G_OBJECT(gridEventBox), "motion-notify-event", G_CALLBACK(fillBoard), NULL);
    g_signal_connect(G_OBJECT(gridEventBox), "button-release-event", G_CALLBACK(addActionCallback), NULL);

    //MARK: Main container
    GtkWidget *mainContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(mainWindow), mainContainer);

    gtk_box_pack_start(GTK_BOX(mainContainer), mainMenu, FALSE, FALSE, 2);
    gtk_box_pack_start(GTK_BOX(mainContainer), tileToolbar, FALSE, FALSE, 2);
    gtk_box_pack_start(GTK_BOX(mainContainer), scrollableArea, TRUE, TRUE, 2);

    gtk_widget_show_all(mainWindow);

    gtk_main();

    return 0;
}

void addActionCallback(GtkWidget *widget, gpointer data){
    if(!addStateToList()){
        showErrorDialog("Błąd pamięci");
    }
}

gboolean shouldCloseWindow(GtkWidget *widget, GdkEvent *event, gpointer data){

    if(!isBoardEdited()){
        gtk_main_quit();
        return TRUE;
    }

    if(showEditionAlertDialog() == GTK_RESPONSE_YES) saveBoardCallback(widget, data);
    else gtk_main_quit();
    return TRUE;
}
