#Proyecto1 (Ronald y Ricardo) Se cambio el orden para que pudiera linkear las librerias
OBJECTS=Example.o
OUTPUT=Example

CFLAGS=-I/usr/local/Mesa-3.4/include
#LDLIBS=-lX11 -lglut -lMesaGLU -lMesaGL -lm -lXext -lXmu
LDLIBS=-lX11 -lglut -lGLU -lGL -lm -lXext -lXmu
LDFLAGS=-L/usr/local/Mesa-3.4/lib -L/usr/X11R6/lib

$(OUTPUT): $(OBJECTS)
	cc $(OBJECTS) -o $(OUTPUT) $(CFLAGS) $(LDFLAGS) $(LDLIBS)

$(OBJECTS):Example.h

clean:
	rm -f *.o
	rm -f Example