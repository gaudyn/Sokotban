#include "GameGrid.h"
#include "../Model/Board.h"
#include "../View-model/BoardLogic.h"


GtkWidget *mainBoardGrid;
int tileSize = 32;

GtkWidget *initBoardGrid() {
    GtkWidget *mainBoardContainer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    mainBoardGrid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(mainBoardGrid), 0);
    gtk_grid_set_column_spacing(GTK_GRID(mainBoardGrid), 0);
    gtk_grid_set_row_homogeneous(GTK_GRID(mainBoardGrid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(mainBoardGrid), TRUE);

    gtk_widget_set_halign(mainBoardGrid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(mainBoardGrid, GTK_ALIGN_CENTER);
    gtk_widget_set_vexpand(mainBoardGrid, TRUE);
    gtk_widget_set_hexpand(mainBoardGrid, TRUE);

    g_signal_connect(G_OBJECT(mainBoardContainer), "draw", G_CALLBACK(mainBoardDraw), NULL);
    gtk_box_pack_start(GTK_BOX(mainBoardContainer), mainBoardGrid, FALSE, FALSE, 2);
    return mainBoardContainer;
}

gboolean mainBoardDraw(GtkWidget *widget, cairo_t *cr, gpointer data){
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    GdkRGBA color;
    gdk_rgba_parse(&color, "#000");

    guint height, width;
    width = gtk_widget_get_allocated_width(widget);
    height = gtk_widget_get_allocated_height(widget);

    gtk_render_background(context, cr, 0, 0, width, height);

    cairo_rectangle(cr, 0, 0, width, height);

    gdk_cairo_set_source_rgba(cr, &color);
    cairo_fill(cr);

    return FALSE;
}

void loadBoardGrid(Board board){

    gtk_container_forall(GTK_CONTAINER(mainBoardGrid), removeWidget, NULL);

    for(int y = 0; y < getBoardSizeY(board); y++){
        for(int x = 0; x < getBoardSizeX(board); x++){

            GtkWidget *cellImage = gtk_image_new();
            gtk_grid_attach(GTK_GRID(mainBoardGrid), cellImage, x, y, 1, 1);
            updateGridTile(x, y, getBoardValueAt(board, x, y));

        }
    }
    gtk_widget_show_all(mainBoardGrid);
}

void updateBoardGrid(Board board){
    for(int y = 0; y < getBoardSizeY(board); y++)
        for(int x = 0; x < getBoardSizeX(board); x++)
            updateGridTile(x, y, getBoardValueAt(board, x, y));
}

void updateGridTile(int posX, int posY, int value){
    GtkWidget *cellImage = gtk_grid_get_child_at(GTK_GRID(mainBoardGrid), posX, posY);
    switch (value)  {
        case BRICK:
            changeImageFile(cellImage, "Tiles/brick.png");
            break;
        case WALK:
            changeImageFile(cellImage, "Tiles/walkable.png");
            break;
        case PLAYER:
            if(getBoardValueAt(getMainBoard(), posX, posY) == BUTTON) changeImageFile(cellImage, "Tiles/player_button.png");
            else changeImageFile(cellImage, "Tiles/player_walk.png");
            break;
        case BOX:
            changeImageFile(cellImage, "Tiles/box.png");
            break;
        case BUTTON:
            changeImageFile(cellImage, "Tiles/button.png");
            break;
        default:
            changeImageFile(cellImage, "Tiles/void.jpg");
            break;
    }
}

int setTileSize(int newSize, Board board){
    if(newSize == -1 && tileSize > 25) tileSize--;
    else if(newSize == 1 && tileSize < 40) tileSize++;
    else if(newSize > 25 && newSize < 40) tileSize = newSize;

    updateBoardGrid(board);

    if(tileSize == 25) return -1;
    if(tileSize == 40) return 1;
    return 0;
}

void changeImageFile(GtkWidget * image, char * file){
    GdkPixbuf *pixbuf;
    pixbuf = gdk_pixbuf_new_from_file_at_size(file, tileSize, -1, NULL);
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
}

void removeWidget(GtkWidget * widget, gpointer data){
    gtk_widget_destroy(widget);
}
