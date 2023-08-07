#pragma once
#include <iostream>
#include <vector>
using namespace std;
const char block_type[][4] = {
	"  ",
	"▣",
	"□",
	"□"
};
class board {
private:
	int point = 0;
	block* myblock = nullptr;
	const static int map_x = 24;
	const static int map_y = 14;
	int base[map_x][map_y] = {};
public:
	board() {
		for (int i = 0; i < map_x; i++) {
			base[i][0] = 1;
			base[i][13] = 1;
		}
		for (int i = 0; i < map_y; i++) {
			base[23][i] = 1;
		}
	}
	void board_print() {
		COORD coor{0, };
		DWORD dw = 0;
		for (int i = 0; i < map_x; i++) {
			int y_pos = 0;
			for (int j = 0; j < map_y; j++) {
				coor.X = y_pos;
				coor.Y = i;
				SetConsoleCursorPosition(console.buffers[console.buffer_now], coor);
				WriteFile(console.buffers[console.buffer_now], block_type[base[i][j]], sizeof(block_type[base[i][j]]), &dw, NULL);
				y_pos += 2;

			}
		}
	}

	bool get_block(int num_init, int shp_init) {
		if (myblock != NULL)
			delete myblock;
		myblock = new block(num_init, shp_init);
		int num = myblock->get_num();
		int shp = myblock->get_shape();
		int pos_x = myblock->get_x();
		int pos_y = myblock->get_y();

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (shape[num][shp][i][j] == 1) {
					if (base[pos_x + i][pos_y + j] != 0)
						return true;
					base[pos_x + i][pos_y + j] = 2;
				}
			}
		}
		return false;
	}

	bool move_block(int move_x, int move_y) {
		if (myblock == NULL)
			return true;
		int num = myblock->get_num();
		int shp = myblock->get_shape();
		int pos_x = myblock->get_x();
		int pos_y = myblock->get_y();
		bool check = false;
		
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (shape[num][shp][i][j] == 1) {
					if (base[pos_x + i + move_x][pos_y + j + move_y] == 1 || base[pos_x + i + move_x][pos_y + j + move_y] == 3) {
						if (move_x > 0) {
							for (int p = 0; p < 4; p++) {
								for (int q = 0; q < 4; q++) {
									if (shape[num][shp][p][q] == 1)
										base[pos_x + p][pos_y + q] = 3;
								}
							}
							delete myblock;
							myblock = nullptr;
							for (int k = 0; k < 4; k++) clear_line(k + pos_x);
						}
						return true;
					}
				}
			}
		}
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (shape[num][shp][i][j] == 1)
					base[pos_x + i][pos_y + j] = 0;
			}
		}
		myblock->set_x(pos_x + move_x);
		myblock->set_y(pos_y + move_y);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (shape[num][shp][i][j] == 1) {
					base[i + pos_x + move_x][j + pos_y + move_y] = 2;
				}
			}
		}
		return false;
	}

	void clear_line(int x) {
		bool check = true;//두줄 한 번에 사라짐 + 위에게 안 움직임
		for (int i = 1; i < map_y - 1; i++) {
			if (base[x][i] != 3) {
				check = false;
				break;
			}
		}

		if (check) {
			point++;
			for (int i = 1; i < map_y - 1; i++) {
				base[x][i] = 0;
			}
			for (int i = x; x >= 1; i--) {
				if (check) {
					for (int j = 1; j < map_y - 1; j++) {
						base[i][j] = base[i - 1][j];
						base[i - 1][j] = 0;
					}
				}
				else {
					break;
				}
     				check = false;
				if (i >= 2) {
					for (int j = 1; j < map_y - 1; j++) {
						if (base[i - 2][j] == 3) {
							check = true;
							break;
						}
					}
				}
			}
		}
	}

	bool shift_shape() {
		if (myblock == NULL)
			return false;
		bool check = true;
		int num = myblock->get_num();
		int shp = myblock->get_shape();
		int pos_x = myblock->get_x();
		int pos_y = myblock->get_y();
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if(shape[num][(shp+1)%4][i][j] == 1) {
					if (base[pos_x + i][pos_y + j] != 0 && base[pos_x + i][pos_y + j] != 2) {
						check = false;
					}
				}
			}
		}
		if (check) {
			myblock->set_shape((shp + 1) % 4);
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (shape[num][shp][i][j] == 1) {
						base[pos_x + i][pos_y + j] = 0;
					}
				}
			}
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (shape[num][(shp + 1) % 4][i][j] == 1) {
						base[pos_x + i][pos_y + j] = 2;
					}
				}
			}
			return true;
		}
		return false;
	}

	void input_key(){
		int key = 0;
		if (_kbhit()){
			key = _getch();
			switch (key){
				case key_code::up:{
					shift_shape();
					break;
				}
				case key_code::down:{
					move_block(1, 0);
					break;
				}
				case key_code::left:{
				move_block(0, -1);
					break;
				}
				case key_code::right: {
					move_block(0, 1);
					break;
				}
				case key_code::space:{
					for (int i = 0; i < 36; i++) move_block(1, 0);
					break;
				}
			}
		}
	}

	void show_point() {
		cout << point << '\n';
	}
};