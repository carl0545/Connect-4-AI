 /*
*Dylan Carlson
*Project 2a
*/

#include <iostream>
#include <math.h>
#include <list>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <time.h>

using namespace std;

#define WIDTH 7
#define HEIGHT 6

typedef struct{
	int heur;
	int key;
} trans;

typedef struct{
	int player;
	int x;
	int y;
} last;

typedef struct{
	int bo[HEIGHT][WIDTH];
	int heur;
	int col;
} node;

class Board{

private:
	int board[HEIGHT][WIDTH]; //0 represents an empty space, 1 - player 1, 2 -player 2
	last lastMove; //contains last move made
	static const int DEPTH = 9; //depth of negamax
	long zobrist[HEIGHT * WIDTH][2]; //Key for transposition table
	list<trans> table; //transposition table

	int search(long); //search transposition table
	long computeKey(int b[HEIGHT][WIDTH]);//computer hash key for transposition table
	long lrand(); //produces random long
	void setLastMove(int x, int y, int player); //Sets the last move instance var every new move
	int eval(int[HEIGHT][WIDTH], int[HEIGHT][WIDTH], int, int, int); //helper for heuristic
	void copyArr(int[HEIGHT][WIDTH], int[HEIGHT][WIDTH]); //copies contents of a source array into a new
	void printArr(int[HEIGHT][WIDTH]); //Print function that utilizes a parameter
	int placeMove(int[HEIGHT][WIDTH], int, int); //returns -1 if column full
	int negamax(node, int, int, int, int);//negamax function that doesn't work
	int minimax(node, int, int, int, int); //minimax function because negamax was confusing me
	void setLastx(int x); //helper for comp move, possibly unneeded
	void setLastRest(int y, int player); //helper for comp move, also possibly unneeded
public:
	Board(); //initializes an empty board
	~Board();
	void printBoard(); //prints current board
	void printLast(); //Used for error checking
	int placeMove(int player, int col); //return -1 if fails, 0 if works
	int isWin(); //checks if the board is in a win state or a tie ( 0 -tie, 1-play1, 2-play2)
	int heuristic(int b[HEIGHT][WIDTH]); //determines state of board
	int compMove(); //Computer's turn

};
