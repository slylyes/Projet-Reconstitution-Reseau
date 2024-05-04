CFLAGS = -g -Wno-unused-parameter
CC = gcc

PROGRAMS = ChaineMain ReconstitueReseau main GrapheMain

.PHONY: all clean

all: $(PROGRAMS)

main.o: main.c
	gcc $(CFLAGS) -c main.c

main: main.o Chaine.o Reseau.o Hachage.o ArbreQuat.o SVGwriter.o
	$(CC) -o $@ $(CFLAGS) $^ -lm

ChaineMain: ChaineMain.o Chaine.o SVGwriter.o
	$(CC) -o $@ $(CFLAGS) $^ -lm

ChaineMain.o: ChaineMain.c
	gcc $(CFLAGS) -c ChaineMain.c

ReconstitueReseau.o: ReconstitueReseau.c
	gcc $(CFLAGS) -c ReconstitueReseau.c

ReconstitueReseau: ReconstitueReseau.o Hachage.o ArbreQuat.o Reseau.o Chaine.o SVGwriter.o
	$(CC) -o $@ $(CFLAGS) $^ -lm

Reseau.o: Reseau.c
	gcc $(CFLAGS) -c Reseau.c

Hachage.o: Hachage.c
	gcc $(CFLAGS) -c Hachage.c

ArbreQuat.o: ArbreQuat.c
	gcc $(CFLAGS) -c ArbreQuat.c

Chaine.o: Chaine.c
	gcc $(CFLAGS) -c Chaine.c

GrapheMain: GrapheMain.o Graphe.o Struct_File.o ArbreQuat.o Reseau.o Chaine.o SVGwriter.o
	$(CC) -o $@ $(CFLAGS) $^ -lm

GrapheMain.o: GrapheMain.c
	gcc $(CFLAGS) -c GrapheMain.c

SVGwriter.o: SVGwriter.c
	gcc $(CFLAGS) -c SVGwriter.c



clean:
	rm -f *.o *.txt *~ $(PROGRAMS) *.html