build_folders:
	mkdir -p build
	mkdir -p levels

web: main.c build_folders
	clang --target=wasm32 --no-standard-libraries -Wl,--export-table -Wl,--no-entry -Wl,--allow-undefined -Wl,--export=main -o web/main.wasm main.c -DPLATFORM_WEB

raylib:	main.c raylib/raylib.c build_folders
	gcc -g -Wall -Wextra -o build/raylib.out raylib/raylib.c main.c -lraylib -lm

raylib/level_editor: raylib/level_editor.c build_folders
	gcc -g -Wall -Wextra -o build/level_editor.out raylib/level_editor.c -lraylib

server: net/server.c
	gcc -g -Wall -Wextra -o build/server.out net/server.c

client: net/client.c
	gcc -g -Wall -Wextra -o build/client.out net/client.c
