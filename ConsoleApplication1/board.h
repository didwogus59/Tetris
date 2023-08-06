#pragma once
#include <iostream>
#include <vector>
using namespace std;
const char BLOCK_TYPES[][4] = {
	"  ",
	"▣",
	"□",
	"□"
};
class board {
	int base[24][14] = {};
	block* myblock = nullptr;
	int map_x = 24;
	int map_y = 14;
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
		COORD coord{ 0, };
		DWORD dw = 0;
		for (int i = 0; i < map_x; i++) {
			int y_pos = 0;
			for (int j = 0; j < map_y; j++) {
				coord.X = y_pos;
				coord.Y = i;
				SetConsoleCursorPosition(console.hBuffer[console.nCurBuffer], coord);
				// 출력 버퍼의 해당 커서 위치에 출력
				WriteFile(console.hBuffer[console.nCurBuffer], BLOCK_TYPES[base[i][j]], sizeof(BLOCK_TYPES[base[i][j]]), &dw, NULL);
				//▣와 □가 띄어쓰기 2개 분량이라 y값을 2씩 이동
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
										base[pos_x + p][pos_y + q] = 3;//다운 말고 다른 상황 고려 수정 필
								}
							}
							delete myblock;
							myblock = nullptr;
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
	void line_check(int x) {
		bool check = true;
		for (int i = 1; i < map_y - 1; i++) {
			if (base[x][i] != 3) {
				check = false;
				break;
			}
		}
		if (check) {
			for (int i = 1; i < map_y - 1; i++) {
				base[x][i] = 0;
			}
			for (int i = x; x >= 0; x--) {
				for (int j = 1; j < map_y - 1; j++) {
					base[x + 1][j] = base[x][j];
				}
			}
		}
	}
	void input_key()
	{
		int nKey = 0;

		// 키입력이 감지되었을 때
		if (_kbhit() > 0)
		{
			// 입력된 키를 받아온다.
			nKey = _getch();

			switch (nKey)
			{
			case eKeyCode::KEY_UP:    // 방향키 위를 눌렀을 때
			{
				break;
			}
			case eKeyCode::KEY_DOWN:  // 방향키 아래를 눌렀을 때
			{
				move_block(1, 0);
				break;
			}
			case eKeyCode::KEY_LEFT:  // 방향키 왼쪽을 눌렀을 때
			{
				move_block(0, -1);
				break;
			}
			case eKeyCode::KEY_RIGHT: // 방향키 오른쪽을 눌렀을 때
			{
				move_block(0, 1);
				break;
			}
			case eKeyCode::KEY_SPACE: // 스페이스바를 눌렀을 때
			{
				for (int i = 0; i < 36; i++) move_block(1, 0);
				break;
			}
			case eKeyCode::KEY_R:     // R키를 눌렀을 때
			{
				break;
			}
			}
		}
	}
};