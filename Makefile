build_folder:
	mkdir -p build

raylib:	main.c raylib/raylib.c build_folder
	gcc -g -Wall -Wextra -o build/main.out raylib/raylib.c main.c -lraylib

raylib/level_editor: raylib/level_editor.c build_folder
	gcc -g -Wall -Wextra -o build/level_editor.out raylib/level_editor.c -lraylib
