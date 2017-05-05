
connect: main.o Board.o
	g++ main.o Board.o -o connect

main.o: main.cpp
	g++ -c main.cpp

Board.o: Board.cpp Board.h
	g++ -c Board.cpp Board.h

clean:
	rm *.o output
