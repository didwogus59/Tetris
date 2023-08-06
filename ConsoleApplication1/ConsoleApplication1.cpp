//#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include "shape.h"
#include "keycode.h"
#include <string>
#include "block.h"
#include "console.h"
#include "board.h"
#include <random>
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dis(0, 6);


void start_game() {
	board* myboard = new board();
	bool check_block = true;
	clock_t cur_time = 0, bef_time = 0;
	myboard->board_print();
	ClearScreen();
	BufferFlip();
	Sleep(1);
	while (1) {
		if (check_block) {
			if (myboard->get_block(dis(gen),dis(gen)%4))
				exit(1);
			check_block = false;
		}
		myboard->input_key();
		myboard->board_print();
		ClearScreen();
		BufferFlip();
		cur_time = clock();
		if (cur_time - bef_time > 500) {
			if (myboard->move_block(1,0)) {
				check_block = true;
			}
			bef_time = clock();
		}
	}
};


int main() {
	InitGame();
	while (true) {
		start_game();
		
	}
}