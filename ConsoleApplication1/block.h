#pragma once

class block {
private:
	int num;
	int shp;
	int now_x, now_y;
public:
	block(int num, int shp) {
		this->num = num;
		this->shp = shp;
		now_x = 0;
		now_y = 5;
	}
	void drop() {
		now_x--;
	}
	void shift(int n) {

	}
	int get_shape() {
		return shp;
	}
	int get_num() {
		return num;
	}
	int get_x() {
		return now_x;
	}
	int get_y() {
		return now_y;
	}
	void set_x(int x) {
		this->now_x = x;
	}
	void set_y(int y) {
		this->now_y = y;
	}
};