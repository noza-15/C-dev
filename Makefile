CC      = gcc
CFLAGS  = -O3
LIBS    = -lGL -lGLU -lglut -lm
OBJS    = main.o game.o hoge.o menu.o obstacle.o ranking.o selector.o huga.o missile.o speed.o background.o player.o raw_data.o score.o
PROGRAM = Flappy-Cube

all:	$(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(PROGRAM)

clean:
	rm -f *.o $(PROGRAM)