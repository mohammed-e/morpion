CC=gcc
CFLAGS=-W -Wall -Wextra -Wswitch-default
LDFLAGS=-lm
EXEC=morpion_console.exe


all: $(EXEC)


$(EXEC): case_t.o grille_t.o ia.o morpion_console.o main.o
	$(CC) $^ -o $@ $(LDFLAGS)


case_t.o: case_t.c case_t.h
	$(CC) -c $< -o $@ $(CFLAGS)


grille_t.o: grille_t.c grille_t.h 
	$(CC) -c $< -o $@ $(CFLAGS)


ia.o: ia.c ia.h
	$(CC) -c $< -o $@ $(CFLAGS)


morpion_console.o: morpion_console.c morpion_console.h
	$(CC) -c $< -o $@ $(CFLAGS)	


main.o: main.c morpion_console.h
	$(CC) -c $< -o $@ $(CFLAGS)	


clean:
	rm -rf *.o *~


mrproper: clean
	rm -rf $(EXEC)


play:
	./$(EXEC)