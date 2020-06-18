CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wno-unused-parameter `pkg-config gtk+-3.0 --cflags`
LFLAGS = `pkg-config gtk+-3.0 --libs`
GAMENAME = SoKOTban
EDITORNAME = SoKOTban-edytor

GAMESRC = MainGame.c View-model/BoardLogic.c Model/Board.c View/MainWindow.c View/GameToolbar.c View/GameGrid.c Model/ActionList.c
EDITORSRC = MainEditor.c View/EditorMenu.c Model/Board.c View-model/BoardOperations.c View/MainWindow.c View/EditorToolbar.c Model/ActionList.c

GAMEDEPS = View-model/BoardLogic.h Model/Board.h View/MainWindow.h View/GameToolbar.h View/GameGrid.h Model/ActionList.h
EDITORDEPS = View/EditorMenu.h Model/Board.h View-model/BoardOperations.h View/MainWindow.h View/EditorToolbar.h Model/ActionList.h

GAMEOBJS = MainGame.o View-model/BoardLogic.o Model/Board.o View/MainWindow.o View/GameToolbar.o View/GameGrid.o Model/ActionList.o
EDITOROBJS = MainEditor.o View/EditorMenu.o Model/Board.o View-model/BoardOperations.o View/MainWindow.o View/EditorToolbar.o Model/ActionList.o

.PHONY = YOU GAME EDITOR clean
YOU: GAME EDITOR
GAME: $(GAMESRC) $(GAMENAME)

$(GAMENAME): $(GAMEOBJS)
	$(CC) $(CFLAGS) $(GAMEOBJS) -o $(GAMENAME) $(LFLAGS)

%.o: %.c $(GAMEDEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

EDITOR: $(EDITORSRC) $(EDITORNAME)

$(EDITORNAME): $(EDITOROBJS)
	$(CC) $(CFLAGS) $(EDITOROBJS) -o $(EDITORNAME) $(LFLAGS)

%.o: %.c $(EDITORDEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(GAMENAME) $(EDITORNAME) $(GAMEOBJS) $(EDITOROBJS)
