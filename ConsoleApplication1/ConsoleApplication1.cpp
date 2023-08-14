#include <conio.h>
#include <ctime>
#include <string>
#include <random>

#include "shape.h"
#include "keycode.h"
#include "block.h"
#include "multi_play.h"
#include "console.h"
#include "board.h"
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dis(0, 6);

void start_single_game() {
	board* myboard = new board();

	board* yourboard = new board();
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
		myboard->input_key();//버퍼를 비워줘야함 안 그럼 이전에 눌렀던게 나옴
		fflush(stdin);
		myboard->board_print();

		yourboard->board_print(0, 40);
		clear_console();
		flip_buffer();
		Sleep(10);
		cur_time = clock();
		if (cur_time - bef_time > 500) {
			if (myboard->move_block(1,0)) {
				check_block = true;
			}
			bef_time = clock();
		}
	}
};

void start_multi_game() {
	board* myboard = new board();
	board* yourboard = new board();
	bool check_block = true;
	clock_t cur_time = 0, bef_time = 0;
	std::cout << "host press 1 or press 2\n";
	int host_user;
	cin >> host_user;

	if (host_user == 1) {
		multi_host* host = new multi_host();
		host->bind_socket();
		host->listen_client();
		host->accept_request();
		host->communication(myboard->get_base(), yourboard->get_base(), myboard->get_map_x(), myboard->get_map_y());
		
		while (1) {
			if (check_block) {
				if (myboard->get_block(dis(gen), dis(gen) % 4)) {
					destroy_buffer();
					cout << myboard->show_point() << '\n';
					break;
				}
				check_block = false;
			}
			myboard->input_key();//버퍼를 비워줘야함 안 그럼 이전에 눌렀던게 나옴
			fflush(stdin);
			myboard->board_print();
			yourboard->board_print(0, 40);
			clear_console();
			flip_buffer();
			host->communication(myboard->get_base(), yourboard->get_base(), myboard->get_map_x(), myboard->get_map_y());
			Sleep(10);
			cur_time = clock();
			if (cur_time - bef_time > 500) {
				if (myboard->move_block(1, 0)) {
					check_block = true;
				}
				bef_time = clock();
			}
		}
	}
	else {
		char* ip = new char[20];
		cout << "please submit server ip\n";
		cin >> ip;
		client* hostee = new client(ip);
		hostee->connect_server();
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
			fflush(stdin);
			myboard->board_print();
			yourboard->board_print(0, 40);
			clear_console();
			flip_buffer();
			hostee->communicate_server(myboard->get_base(), yourboard->get_base(),myboard->get_map_x(),myboard->get_map_y());
			Sleep(10);
			cur_time = clock();
			if (cur_time - bef_time > 500) {
				if (myboard->move_block(1, 0)) {
					check_block = true;
				}
				bef_time = clock();
			}
		}
	}
}
int main() {
	init_buffer();
	int n;
	cout << "Press 1 Single game\nPress 2 Multi game\n";
	while (cin >> n) {
		if (n == 1) {
			start_single_game();
			break;
		}
		if (n == 2) {
			start_multi_game();
			break;
		}
		cout << "Press 1 Single game\n Press 2 Multi game\n";
	}
	start_single_game();
}