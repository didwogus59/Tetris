#include <windows.h>
#include <conio.h>
#include <ctime>
#include <string>
#include <random>

#include "shape.h"
#include "keycode.h"
#include "block.h"
#include "console.h"
#include "board.h"

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dis(0, 6);

void start_game() {
	board* myboard = new board();
	bool check_block = true;
	clock_t cur_time = 0, bef_time = 0;
	while (1) {
		if (check_block) {
			if (myboard->get_block(dis(gen), dis(gen) % 4)) {
				destroy_buffer();
				myboard->show_point();
				break;
			}
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
	init_buffer();
	start_game();
}