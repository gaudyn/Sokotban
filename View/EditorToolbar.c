#include "EditorToolbar.h"

#include <gtk/gtk.h>

#include "../Model/Board.h"

int currentTile;
GtkToolItem *description;

GtkWidget *initToolbar() {
    GtkWidget *tileToolbar = gtk_toolbar_new();
    gtk_toolbar_set_style(GTK_TOOLBAR(tileToolbar), GTK_TOOLBAR_ICONS);

    GSList * group = NULL;

    GtkWidget *emptyTileIcon = gtk_image_new_from_file("Tiles/void.jpg");
    GtkToolItem *emptyTile = gtk_radio_tool_button_new(group);
    gtk_tool_button_set_icon_widget(GTK_TOOL_BUTTON(emptyTile), emptyTileIcon);
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(emptyTile), "Puste pole");
    gtk_tool_item_set_homogeneous(emptyTile, TRUE);
    g_signal_connect(G_OBJECT(emptyTile), "clicked", G_CALLBACK(setCurrentTile), GINT_TO_POINTER(EMPTY));

    GtkWidget *walkTileIcon = gtk_image_new_from_file("Tiles/walkable.png");
    GtkToolItem *walkTile = gtk_radio_tool_button_new_from_widget(GTK_RADIO_TOOL_BUTTON(emptyTile));
    gtk_tool_button_set_icon_widget(GTK_TOOL_BUTTON(walkTile), walkTileIcon);
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(walkTile), "Podłoga");
    gtk_tool_item_set_homogeneous(walkTile, TRUE);
    g_signal_connect(G_OBJECT(walkTile), "clicked", G_CALLBACK(setCurrentTile), GINT_TO_POINTER(WALK));

    GtkWidget *brickTileIcon = gtk_image_new_from_file("Tiles/brick.png");
    GtkToolItem *brickTile = gtk_radio_tool_button_new_from_widget(GTK_RADIO_TOOL_BUTTON(emptyTile));
    gtk_tool_button_set_icon_widget(GTK_TOOL_BUTTON(brickTile), brickTileIcon);
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(brickTile), "Ściana");
    gtk_tool_item_set_homogeneous(brickTile, TRUE);
    g_signal_connect(G_OBJECT(brickTile), "clicked", G_CALLBACK(setCurrentTile), GINT_TO_POINTER(BRICK));

    GtkWidget *playerTileIcon = gtk_image_new_from_file("Tiles/player_walk.png");
    GtkToolItem *playerTile = gtk_radio_tool_button_new_from_widget(GTK_RADIO_TOOL_BUTTON(emptyTile));
    gtk_tool_button_set_icon_widget(GTK_TOOL_BUTTON(playerTile), playerTileIcon);
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(playerTile), "Gracz");
    gtk_tool_item_set_homogeneous(playerTile, TRUE);
    g_signal_connect(G_OBJECT(playerTile), "clicked", G_CALLBACK(setCurrentTile), GINT_TO_POINTER(PLAYER));

    GtkWidget *boxTileIcon = gtk_image_new_from_file("Tiles/box.png");
    GtkToolItem *boxTile = gtk_radio_tool_button_new_from_widget(GTK_RADIO_TOOL_BUTTON(emptyTile));
    gtk_tool_button_set_icon_widget(GTK_TOOL_BUTTON(boxTile), boxTileIcon);
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(boxTile), "Pudełko");
    gtk_tool_item_set_homogeneous(boxTile, TRUE);
    g_signal_connect(G_OBJECT(boxTile), "clicked", G_CALLBACK(setCurrentTile), GINT_TO_POINTER(BOX));

    GtkWidget *buttonTileIcon = gtk_image_new_from_file("Tiles/button.png");
    GtkToolItem *buttonTile = gtk_radio_tool_button_new_from_widget(GTK_RADIO_TOOL_BUTTON(emptyTile));
    gtk_tool_button_set_icon_widget(GTK_TOOL_BUTTON(buttonTile), buttonTileIcon);
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(buttonTile), "Pole docelowe");
    gtk_tool_item_set_homogeneous(buttonTile, TRUE);
    g_signal_connect(G_OBJECT(buttonTile), "clicked", G_CALLBACK(setCurrentTile), GINT_TO_POINTER(BUTTON));

    description = gtk_tool_button_new(NULL, "Puste pole");
    gtk_tool_item_set_homogeneous(description, FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(description), FALSE);

    gtk_toolbar_insert(GTK_TOOLBAR(tileToolbar), emptyTile, 0);
    gtk_toolbar_insert(GTK_TOOLBAR(tileToolbar), walkTile, -1);
    gtk_toolbar_insert(GTK_TOOLBAR(tileToolbar), brickTile, -1);
    gtk_toolbar_insert(GTK_TOOLBAR(tileToolbar), gtk_separator_tool_item_new(), -1);
    gtk_toolbar_insert(GTK_TOOLBAR(tileToolbar), playerTile, -1);
    gtk_toolbar_insert(GTK_TOOLBAR(tileToolbar), boxTile, -1);
    gtk_toolbar_insert(GTK_TOOLBAR(tileToolbar), buttonTile, -1);
    gtk_toolbar_insert(GTK_TOOLBAR(tileToolbar), gtk_separator_tool_item_new(), -1);
    gtk_toolbar_insert(GTK_TOOLBAR(tileToolbar), description, -1);
    return tileToolbar;
}

void setCurrentTile(GtkWidget * widget, gpointer tile){
    currentTile = GPOINTER_TO_INT(tile);
    switch(currentTile){
        case WALK:
            gtk_tool_button_set_label(GTK_TOOL_BUTTON(description), "Wybrana płytka: podłoga");
            break;
        case BRICK:
            gtk_tool_button_set_label(GTK_TOOL_BUTTON(description), "Wybrana płytka: ściana");
            break;
        case PLAYER:
            gtk_tool_button_set_label(GTK_TOOL_BUTTON(description), "Wybrana płytka: gracz");
            break;
        case BOX:
            gtk_tool_button_set_label(GTK_TOOL_BUTTON(description), "Wybrana płytka: pudełko");
            break;
        case BUTTON:
            gtk_tool_button_set_label(GTK_TOOL_BUTTON(description), "Wybrana płytka: pole docelowe");
            break;
        default:
            gtk_tool_button_set_label(GTK_TOOL_BUTTON(description), "Wybrana płytka: puste pole");
    }
}
