
/*
*Dylan Carlson
*Project 2a
*/

#include <iostream>
#include <time.h>
#include "Board.h"

using namespace std;
int main(int argc, char *argv[]) {
	srand(time(NULL));

	////////INITIAL DECLARATIONS//////
	Board b;
	int user = 0;
	int player = 1;


	///////MAIN LOOP///////

	while(true){
		b.printBoard();
		cout << "Player " << player << " what column would you like to place in: ";
		if(player == 2){
		cin >> user;
		if(user == -1) break;
		while(user < 0 || user > 6){
			cout << "Input out of bounds, re-enter column: ";
			cin >> user;
		}
		int flag = b.placeMove(player, user);
		while(flag == -1){
			cout << "Column is full please pick another column: ";
			cin >> user;
			flag = b.placeMove(player, user);

			}
		}
		else{
			b.placeMove(player, b.compMove());

		}
	    int win = b.isWin();
		if(win == 1){
			b.printBoard();
			cout << "Player 1 wins!" << endl;
			break;
		}
		if(win == 2){
			b.printBoard();
			cout << "Player 2 wins!" << endl;
			break;
		}
		if(win == 0){
			b.printBoard();
			cout << "It is a tie!" << endl;
			break;
		}

		if(player == 1) player = 2;
		else player = 1;
	}

	b.printLast();
	return 0;
}
