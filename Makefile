clean:
	rm *.out

game:
	gcc main.c -o game.out \
		utils.c \
		renderer.c \
		entities/player/player.c \
		entities/enemy/enemy.c \
		entities/card/card.c \
		core/core.c \
		images/load-images.c \
		-lm -g\
		-I. \
		`pkg-config allegro-5 allegro_main-5 \
		allegro_font-5 \
		allegro_image-5 allegro_primitives-5 allegro_ttf-5 \
		 --libs --cflags`