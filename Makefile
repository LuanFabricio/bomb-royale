build_folders:
	mkdir -p build
	mkdir -p levels

raylib:	main.c raylib/raylib.c build_folders
	gcc -g -Wall -Wextra -o build/raylib.out raylib/raylib.c main.c -lraylib

raylib/level_editor: raylib/level_editor.c build_folders
	gcc -g -Wall -Wextra -o build/level_editor.out raylib/level_editor.c -lraylib
