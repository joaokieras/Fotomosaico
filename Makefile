CFLAG = -Wall
LIBS = -lm
CC = gcc
EXEC = mosaico
MAIN = mosaico.c
INCLUDE = imagens.c
OBJS = mosaico.o imagens.o

all: hello

hello: $(OBJS)
	$(CC) -o $(EXEC) $(MAIN) $(INCLUDE) $(LIBS) $(CFLAG)

mosaico.o: mosaico.c imagens.h 
imagens.o: imagens.c imagens.h 

clean:
	-rm -f *~ *.o

purge:
	-rm -f $(OBJS) $(EXEC)