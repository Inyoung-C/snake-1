#ifndef FIELD_H
#define FIELD_H


#define FIELD 0
#define SNAKE 1
#define FOOD  2
#define FRAME 3
#define NOFRAME 4

#define MAX_SPEED 10

#include <queue>
 
/* ������������ ���������:

      *-----------------> "y"
      |
      |
      |
      |
      |
 "x"  Y 

*/


struct Point {
	
	bool operator==(Point p);
	
	//Point(size_t x, size_t y);
	
	size_t x;
	size_t y;
};


struct Snake {
	int start_len; // ��������� ����� ����
	Point head; // ���������� ������
	Point tail; // ���������� �������
	std::queue <Point> twists; // �������� ������
	int speed; // �������� ����. �� 0 (��������) �� 10 (������).
};

struct Field {

	Field(size_t x, size_t y, int frame, size_t snake_len, int speed);
	~Field();
	void print_field(int x, int y); // ������������ ���� � ���������� � ����������� (x, y)
	size_t get_x();
	size_t get_y();
	int move();
	void add_food();
	
	char * field_copy();
	
private:
	
	Snake snake;
	char ** field; // ���� ������� �� �����. ������ - �� ������. ��� - �� �����. ����� - �� �����.
	size_t x; // ����� (�������������� ������������)
	size_t y; // ������ (������������ ������������)
	
	unsigned /*unsigned - �����������*/int frame; // ������� ����� (0 - ���, 1 - ����).
};

#endif
