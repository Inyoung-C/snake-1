#ifndef FIELD_H
#define FIELD_H


#define FIELD 0
#define SNAKE 1
#define FOOD  2
#define FRAME 3
#define NOFRAME 4

#define MAX_SPEED 10

#include <queue>
 
/* Координатная плоскость:

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
	int start_len; // стартовая длина змеи
	Point head; // координаты головы
	Point tail; // координаты хваоста
	std::queue <Point> twists; // повороты змейки
	int speed; // скорости змеи. от 0 (медленно) до 10 (быстро).
};

struct Field {

	Field(size_t x, size_t y, int frame, size_t snake_len, int speed);
	~Field();
	void print_field(int x, int y); // отрисовывает поле с знакоместа с координатой (x, y)
	size_t get_x();
	size_t get_y();
	int move();
	void add_food();
	
	char * field_copy();
	
private:
	
	Snake snake;
	char ** field; // поле состоит из нулей. Змейка - из единиц. Еда - из двоек. Рамка - из троек.
	size_t x; // длина (горизонтальная составляющая)
	size_t y; // ширина (вертикальная составляющая)
	
	unsigned /*unsigned - обязательно*/int frame; // наличие рамки (0 - нет, 1 - есть).
};

#endif
