//
// Board.cpp
// Created by D-Money Jenkins on 2/24/17.

#include "Board.h"

Board::Board() {
	for(int i = 0; i < HEIGHT; i++){
		for(int j = 0; j < WIDTH; j++){
			board[i][j] = 0;
		}
	}

	lastMove.player = 0;
	lastMove.x = -1;
	lastMove.y = -1;



	for (int square = 0; square < HEIGHT*WIDTH; square++)
		for (int side = 0; side < 2; side++)
			zobrist[square][side] = lrand();
	
}
long Board::computeKey(int b[HEIGHT][WIDTH]){
	long hashKey = 0;

	for(int i = 0; i < HEIGHT; i++){
		for(int j = 0; j < WIDTH; j++){
			int side = b[i][j] - 1;
			if(side > -1){
				hashKey ^= zobrist[i*j][side];
			}
		}
	}
	return hashKey;
}
int Board::search(long key){

	for (std::list<trans>::iterator it=table.begin(); it!=table.end(); ++it){
		trans ex = *it;
		if(ex.key == key) return ex.heur;
	}
	return -1;

}

long Board::lrand(){
	if (sizeof(int) < sizeof(long))
			return (static_cast<long>(rand()) << (sizeof(int) * 8)) |
				   rand();

		return rand();

}


void Board::printBoard(){
	for(int k = 0; k < WIDTH; k++){
		cout << k << " ";
	}
	cout << endl;

	for(int i = 0; i < HEIGHT; i++){
		for(int j = 0; j < WIDTH; j++){
			char num;

			if(board[i][j] == 0) num = '-';
			if(board[i][j] == 2) num = 'X';
			if(board[i][j] == 1) num = 'O';

			cout << num << " ";
		}
		cout << endl;
	}
	int h = heuristic(board);
	cout << "heuristic: " << h << endl;
}

int Board::placeMove(int player, int col){
	int flag = 0;
	for(int i = HEIGHT-1; i >= 0; i--){
		if(board[i][col] == 0){
			board[i][col] = player;
			flag = 1;
			setLastMove(i, col, player);
			break;
		}
	}
	if(flag == 0){
		return -1;
	}

	return 0;
}

void Board::setLastMove(int x, int y, int player){
	lastMove.x = x;
	lastMove.y = y;
	lastMove.player = player;

}
void Board::printLast(){
	cout << "x: "<< lastMove.x << " y: " << lastMove.y << " Player: " << lastMove.player << endl;

}

int Board::isWin(){
	///TIE CHECK//
	int flag = 0;
	for(int j = 0; j < WIDTH; j++){
		if(board[0][j] == 0) break;
		if(j == WIDTH-1) flag = 1;
	}
	if(flag == 1) {
		return 0;
	}

	//////////////////
	int x,y, player, sum;
	x = lastMove.x;
	y = lastMove.y;
	player = lastMove.player;
	sum = 1;


	////SIDE CHECK/////

	for(int i = y-1; i >= 0; i--){
		if(board[x][i] == player) sum++;
		else break;
	}

	for(int j = y+1; j < WIDTH; j++){
		if(board[x][j] == player) sum++;
		else break;
	}

	if(sum >= 4){
		cout << endl << "side win"<< endl;
	 return player;
	}

	/////VERTICAL CHECK//////
	sum = 1;

	for(int i = x-1; i >= 0; i--){
		if(board[i][y] == player) sum++;
		else break;
	}

	for(int j = x+1; j < HEIGHT; j++){
		if(board[j][y] == player) sum++;
		else break;
	}

	if(sum >= 4){
		 cout << endl << "vertical win" << endl;
		 return player;
	}

	/////RIGHT DIAGONAL CHECK//////
	sum = 1;
	int k = y+1;

	for(int i = x-1; i >= 0; i--){
		if(k >= WIDTH) break;
		if(board[i][k] == player) sum++;
		else break;
		k++;
	}

	k = y-1;

	for(int j = x+1; j < HEIGHT; j++){
		if(k < 0) break;
		if(board[j][k] == player) sum++;
		else break;
		k--;
	}

	if(sum >= 4){
		 cout << endl << "right diagonal win" << endl;
		 return player;
	}

	//////LEFT DIAGONAL CHECK//////
	sum = 1;
	k = y-1;

	for(int i = x-1; i >= 0; i--){
		if(k < 0) break;
		if(board[i][k] == player) {
			cout << endl << "x: " << i << " y: " << k << endl;
			sum++;
		}
		else break;
		k--;
	}

	k = y+1;

	for(int j = x+1; j < HEIGHT; j++){
		if(k >= WIDTH) break;
		if(board[j][k] == player) {
			cout << endl << "2x: " << j << " 2y: " << k << endl;
			sum++;
		}
		else break;
		k++;
	}

	if(sum >= 4) {
		cout << endl << "left diag win" << endl;
		return player;
	}

return -1;
}

int Board::heuristic(int b[HEIGHT][WIDTH]){
	int hvalue = 0;
	int checked[HEIGHT][WIDTH];
	int newE = 0;

	for(int i = 0; i < HEIGHT; i++){
		for(int j = 0; j < WIDTH; j++){
			checked[i][j] = 0;
		}
	}

	for(int i = 0; i < HEIGHT; i++){
		for(int j = 0; j < WIDTH; j++){
			if(checked[i][j] == 1) continue;
			if(b[i][j] == 0) continue;

			if(b[i][j] == 1){
				newE = eval(b, checked, i, j, 1);
				hvalue += newE;
				if(newE > 10000) return hvalue;
			}
			if(b[i][j] == 2){
				newE = eval(b, checked, i, j, 2);
				hvalue -= newE;
				if(newE > 10000) return hvalue;
			}
		}
	}


	return hvalue;
}

int Board::eval(int b[HEIGHT][WIDTH], int checked[HEIGHT][WIDTH], int i, int j, int player){
	int eval = 0;
	int opp;
	int sum = 1;
	int flag = 0;

	if(player == 1) opp = 2;
	else opp = 1;

	///horizontal check////


	for(int q = j - 1; q > 0; q--){
		if(b[i][q] == player){
			sum++;
			checked[i][q] = 1;
		}
		if(b[i][q] == opp){
			flag++;
			break;
		}
		if(b[i][q] == 0){
			break;
		}

	}



	for(int q = j + 1; q < WIDTH; q++){
		if(b[i][q] == player){
			sum++;
			checked[i][q] = 1;
		}
		if(b[i][q] == opp){
			flag++;
			break;
		}
		if(b[i][q] == 0){
			break;
		}
	}

	if(flag == 2) sum = 1;
	if(sum >= 4) {
		eval = 100000;
		return eval;
	}
	if(sum == 3) eval += 300;
	if(sum == 2) eval += 15;

	//////vertical check///////
	sum = 1;
	flag = 0;
	/* not needed redundant
	for(int q = j-1; q >= 0; q--){
		if(b[q][j] == player){
			sum++;
			checked[q][j] = 1;
		}
		if(b[q][j] == opp){
			flag++;
			break;
		}
		if(b[q][j] == 0){
			break;
		}
	}
	*/

	for(int q = i+1; q < HEIGHT; q++){
		if(b[q][j] == player){
			sum++;
			checked[q][j] = 1;
		}
		if(b[q][j] == opp){
			flag++;
			break;
		}
		if(b[q][j] == 0){
			break;
		}
	}
	if(flag == 2) sum = 1;
		if(sum >= 4) {
			eval = 100000;
			return eval;
		}
		if(sum == 3) eval += 300;
		if(sum == 2) eval += 15;

///////Right diagonal check/////////
	sum = 1;
	flag = 0;
	int k = j - 1;

	for(int q = i - 1; q >= 0; q--){
		if(k < 0) break;

		if(b[q][k] == player){
			sum++;
			checked[q][k] = 1;
		}
		if(b[q][k] == opp){
			flag++;
			break;
		}
		if(b[q][k] == 0){
			break;
		}

		k--;
	}

	k = j + 1;

	for(int q = i + 1; q < HEIGHT; q++){
		if(k >= WIDTH) break;
		if(b[q][k] == player){
			sum++;
			checked[q][k] = 1;
		}
		if(b[q][k] == opp){
			flag++;
			break;
		}
		if(b[q][k] == 0){
			break;
		}

		k++;
	}

	if(flag == 2) sum = 1;
	if(sum >= 4) {
		eval = 100000;
		return eval;
	}
	if(sum == 3) eval += 100;
	if(sum == 2) eval += 5;

	////LEFT DIAGONAL///////
	sum = 1;
	flag = 0;
	k = j + 1;

	for(int q = i - 1; q >= 0; q--){
		if(k >= WIDTH) break;
		if(b[q][k] == player){
			sum++;
			checked[q][k] = 1;
		}
		if(b[q][k] == opp){
			flag++;
			break;
		}
		if(b[q][k] == 0){
			break;
		}
		k++;
	}

	k = j - 1;

	for(int q = i + 1; q < HEIGHT; q++){
		if(k < 0) break;
		if(b[q][k] == player){
			sum++;
			checked[q][k] = 1;
		}
		if(b[q][k] == opp){
			flag++;
			break;
		}
		if(b[q][k] == 0){
			break;
		}
		k--;
	}

	if(flag == 2) sum = 1;
	if(sum >= 4) {
		eval = 100000;
		return eval;
	}
	if(sum == 3) eval += 100;
	if(sum == 2) eval += 5;


	return eval;
}
int Board::placeMove(int bo[HEIGHT][WIDTH], int move, int player){
	int flag = 0;
	int i;
		for(i = HEIGHT-1; i >= 0; i--){
			if(bo[i][move] == 0){
				bo[i][move] = player;
				flag = 1;
				setLastx(i);
				break;
			}
		}
		if(flag == 0){
			return -1;
		}


		return i;
}

void Board::setLastx(int x){
	lastMove.x = x;
}

void Board::setLastRest(int y, int player){
	lastMove.y = y;
	lastMove.player = player;
}

int Board::compMove(){
	int move = -1;
	int depth = DEPTH;
	int n;
	int last = -1000000;
	node current;
	int flag;

	copyArr(board, current.bo);
	/*
	int tryh = search(computeKey(current.bo));//
	if(tryh != -1){//
		current.heur = tryh;//
	}//
	else{//
	*/
	current.heur = heuristic(current.bo);
	current.col = -1;
	/*
	trans node; //
	node.key = computeKey(current.bo);//
	node.heur = current.heur;//
	table.push_front(node);//
	}//
	*/

	for(int col = 0; col < WIDTH; col++){
		int good;
		flag = 0;
		if(current.bo[0][col] == 0){
			good = placeMove(current.bo, col, 1);
			if(good == -1) continue;
			flag = 1;
			//n = heuristic(current.bo);
			//n = negamax(current, depth-1, INT_MAX, INT_MIN, -1);
			n = minimax(current, depth-1, 2, INT_MIN, INT_MAX);
			cout << endl << "n: " << n;
			if(n > last){
				last = n;
				move = col;
			}
			if(flag == 1){
				current.bo[good][col] = 0;
			}


		}
	}

	setLastRest(move, 1);
	cout << endl;
	return move;
}

int Board::minimax(node curr, int depth, int turn, int alpha, int beta){
	int bestValue;
	int heur;

	/*
	int tryh = search(computeKey(curr.bo));//
	if(tryh != -1){//
		curr.heur = tryh;//
	}//
	else{//
	trans node;//
	*/
	curr.heur = heuristic(curr.bo);
	/*
	node.heur = curr.heur;//
	node.key = computeKey(curr.bo);//
	table.push_front(node);//
	}//
	*/
	heur = curr.heur;


	///TERMINAL///
	if(depth == 0){
		return heur;
	}
	if(heur > 8000){
		return heur;
	}
	if(heur < -8000){
		return heur;
	}

	if(turn == 1){
		bestValue = INT_MIN;

		for(int col = 0; col < WIDTH; col++){
			int flag = 0;
			int good;
			node newnode;
			copyArr(curr.bo, newnode.bo);
			int n;

			if(newnode.bo[0][col] == 0){
				good = placeMove(newnode.bo, col, turn);
				if(good == -1) continue;
				flag = 1;
				n = minimax(newnode, depth-1, 2, alpha, beta);
				bestValue = max(bestValue, n);
				if(bestValue > alpha){
					alpha = bestValue;
				}
				if(alpha >= beta){
					return alpha;
				}
			}
		}
		return alpha;

	}
	else{
		bestValue = INT_MAX;

		for(int col = 0; col < WIDTH; col++){
			int flag = 0;
			int good;
			node newnode;
			copyArr(curr.bo, newnode.bo);
			int n;

			if(newnode.bo[0][col] == 0){
				good = placeMove(newnode.bo, col, turn);
				if(good == -1) continue;
				flag = 1;
				n = minimax(newnode, depth-1, 1, alpha, beta);
				bestValue = min(bestValue, n);
				if(bestValue < beta){
					beta = bestValue;
				}
				if(beta <= alpha){
					return beta;
				}

			}

		}

		return beta;
	}

}

int Board::negamax(node curr, int depth, int alpha, int beta, int color){
	int bestValue = INT_MIN;
	int heur;
	int newColor;
	curr.heur = heuristic(curr.bo);
	heur = curr.heur;

	if(color == 1) newColor = 2;
	else newColor = 1;

	////Terminal////
	if(depth == 0){
		return heur;
	}

	if(heur > 1000){
		return heur;
	}
	if(heur < -1000){
		return heur;
	}
	//////////////////

	for(int col = 0; col < WIDTH; col++){
		int flag = 0;
		int good;
		node newnode;
		copyArr(curr.bo, newnode.bo);
		int n;

		if(newnode.bo[0][col] == 0){
			good = placeMove(newnode.bo, col, color);
			if(good == -1) continue;
			flag = 1;
			n = negamax(newnode, depth -1, -beta, -alpha, newColor);
			bestValue = max(bestValue, n);
			alpha = max(alpha, n);
			/*
			if(alpha >= beta)
				break;
				*/
		}


	}





	return bestValue;
}

void Board::copyArr(int source[HEIGHT][WIDTH], int newS[HEIGHT][WIDTH]){

	for(int i = 0; i < HEIGHT; i++){
		for(int j = 0; j < WIDTH; j++){
			newS[i][j] = source[i][j];
		}
	}


}

void Board::printArr(int bo[HEIGHT][WIDTH]){
	for(int i = 0; i < HEIGHT; i++){
			for(int j = 0; j < WIDTH; j++){
				char num;

				if(bo[i][j] == 0) num = '-';
				if(bo[i][j] == 2) num = 'X';
				if(bo[i][j] == 1) num = 'O';

				cout << num << " ";
			}
			cout << endl;
		}
}

Board::~Board() {

}
