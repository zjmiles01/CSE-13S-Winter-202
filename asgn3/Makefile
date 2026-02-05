HEADERS = game.h randi.h strings.h

lcr: lcr.o game.o randi.o
	clang lcr.o game.o randi.o -o lcr

lcr.o: lcr.c $(HEADERS)
	clang -c -Wall -Werror lcr.c

game.o: game.c $(HEADERS)
	clang -c -Wall -Werror game.c

randi.o: randi.c randi.h
	clang -c -Wall -Werror randi.c

clean:
	rm -f *.o lcr