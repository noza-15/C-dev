# this makefile is for macOS users

# c compiler is Apple LLVM
CC	= /usr/bin/gcc
CFLAGS	= -Wno-deprecated
LDLIBS	= -framework GLUT -framework OpenGL
OBJECTS	= $(patsubst %.c,%.o,$(wildcard *.c))

# define the aplication name
TARGET	= myapp

$(TARGET): $(OBJECTS)
	$(CC) $^ $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $*.o $*.c

.PHONY: rebuild clean

rebuild:
	make clean
	make

clean:
	$(RM) $(TARGET) $(OBJECTS)

################################################################################

# macで<GL/glut.h>が使えるようにする
# !warning : this command is dangerous
makePathToGLUT:
	sudo mkdir /usr/local/include/GL
	sudo ln -s /System/Library/Frameworks/GLUT.framework/Headers/glut.h /usr/local/include/GL

# macに無理やり作った<GL/glut.h>を消す
# !warning : this command is dangerous
removePathToGLUT:
	sudo rm -rf /usr/local/include/GL
