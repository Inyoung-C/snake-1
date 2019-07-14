#include <curses.h>
#include <cstdlib>
#include <ctime>
#include <queue>
#include "field.h"
#include "header.h"

bool Point::operator==(Point p) {
	return this->x == p.x && this->y == p.y;
}

/*Point::Point(size_t a, size_t b) {
	x = a;
	y = b;
}*/


/* 
			!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
							СДЕЛАТЬ СТРУКТУРУ POINT С ПОКАЗАТЕЛЕМ ТОГО, ЯВЛЯЕТСЯ ЛИ ЭТА ТОЧКА ТОЧКОЙ РАЗРЫВА
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

*/

char * Field::field_copy() {
	static char * arr = new char[x * y];
	for (size_t i = 0; i < x; ++i) {
		for (size_t j = 0; j < y; ++j) {
			arr[i*x + j] = field[i][j];
		}
	}
	return arr;
}

int foo(int a) { // ну а как еще назвать это чудо?
	if (a > 0) return 1;
	if (a < 0) return -1;
	if (a = 0) return 0;
}

void new_pos(size_t & pos_x, size_t & pos_y, int change_x, int change_y, int size_x, int size_y, int frame, std::queue <Point> & twists) { // меняет координату pos на change. 
	
	fprintf(log, "changes: (%3d, %3d)\n", change_x, change_y);
	fprintf(log, "x: %3d -> %3d\n", pos_x, (pos_x + change_x) % (size_x - 1)); // баг из единицы попадаю в единицу (см лог)
	//fprintf(log, "(  %3d -> (%d + %d) %% (%d - 1))\n", pos_x, pos_x, change_x, size_x);
	fprintf(log, "y: %3d -> %3d\n", pos_y, (pos_y + change_y) % (size_y - 1));
	int new_pos_x = (pos_x + change_x) % (size_x - 1);
	int new_pos_y = (pos_y + change_y) % (size_y - 1);
	Point p = {pos_x, pos_y};
	
	if (new_pos_x == 0 && frame == 0) { 
		if (pos_x == size_x - 2)
			new_pos_x = 1;
		else
			new_pos_x = size_x - 2;
		twists.push(p);
		Point p1 = {new_pos_x, new_pos_y};
		twists.push(p1);
	}
	if (new_pos_y == 0 && frame == 0) { 
		if (pos_y == size_y - 2)
			new_pos_y = 1;
		else
			new_pos_y = size_y - 2;
		twists.push(p);
		Point p1 = {new_pos_x, new_pos_y};
		twists.push(p1);
	}
	fprintf(log, "\tx: %3d -> %3d\n", pos_x, new_pos_x);
	fprintf(log, "\ty: %3d -> %3d\n", pos_y, new_pos_y);
	fflush(log);
	if (change_x) pos_x = new_pos_x;
	if (change_y) pos_y = new_pos_y;
}

Field::Field(size_t x, size_t y, int frame, size_t snake_len, int speed) {
	// Генерируем поле и рамку (если требуется)
	this->x = x;
	this->y = y;
	this->frame = frame;
	field = new char* [x];
	for (int i = 0; i < x; ++i) {
		field[i] = new char[y];
		for (int j = 0; j < y; ++j) {
			field[i][j] = FIELD;
			if (i == 0 || i + 1 == x || j == 0 || j + 1 == y) {
				if (frame)
					field[i][j] = FRAME;
				else
					field[i][j] = NOFRAME;
			}
			
		}
	}
	fprintf(log, ">>> char ** field created\n");
	// генерируем змею
	srand(time(NULL));
	snake.head = {rand() % (this->get_x() - 2) + 1, rand() % (this->get_y() - 2) + 1};
	while (1) {
		int direction = rand() % 4; // 0 - вниз. 1 - вправо. 2 - вверх. 3 - влево. (направление, куда закинуть хвост)
		if (direction == 0) {
				fprintf(log, "(0)\n");
				if (snake.head.x + snake_len < ((int)x - foo(frame))) {
					snake.tail = {snake.head.x + snake_len, snake.head.y};
					break;
				}
		}
		else if (direction == 1) {
				fprintf(log, "(1)\n");
				if (snake.head.y + snake_len < ((int)y - foo(frame))) {
					snake.tail = {snake.head.x, snake.head.y + snake_len};
					break;
				}
		}
		else if (direction == 2) {
				fprintf(log, "(2)\n");
				if (((int)snake.head.x - (int)snake_len) > 0 + foo(frame)) {
					fprintf(log, "%d > %d\n", (int)snake.head.x - snake_len, 0 + foo(frame));
					snake.tail = {snake.head.x - snake_len, snake.head.y};
					break;
				}
		}
		else if (direction == 3) {
				fprintf(log, "(3)\n");
				if (((int)snake.head.y - (int)snake_len) > 0 + foo(frame)) {
					fprintf(log, "%d > %d\n", (int)snake.head.y - (int)snake_len, 0 + foo(frame));
					snake.tail = {snake.head.x, snake.head.y - snake_len};
					break;
				}
		}
		else {
			fprintf(log, "!!! something impossible happened\n");
			exit(0);
		} 
	}
	
	fprintf(log, "Snake generated: head = (%d, %d). tail = (%d, %d)\n", snake.head.x, snake.head.y, snake.tail.x, snake.tail.y);
	fprintf(log, "Painting Snake:\n");
	fflush(log);
	
	if (snake.head.x > snake.tail.x) { // если змея вертикальна, и голова направлена вниз
		fprintf(log, "(1)\n");
		for (int i = snake.tail.x; i <= snake.head.x; ++i) {
			for (int j = snake.head.y; j <= snake.tail.y; ++j) {
				field[i][j] = SNAKE;
				fprintf(log, "%d %d\n", i, j);
			}
		}
	}
	else if (snake.head.x < snake.tail.x) { // если змея вертикальна, и голова направлена вверх
		fprintf(log, "(2)\n");
		for (int i = snake.tail.x; i >= snake.head.x; --i) {
			for (int j = snake.head.y; j <= snake.tail.y; ++j) {
				field[i][j] = SNAKE;
				fprintf(log, "%d %d\n", i, j);
			}
		}
	}
	if (snake.head.y > snake.tail.y) { // если змея горизонтальна, и голова направлена вправо
		fprintf(log, "(3)\n");
		for (int i = snake.tail.x; i <= snake.head.x; ++i) {
			for (int j = snake.tail.y; j <= snake.head.y; ++j) {
				field[i][j] = SNAKE;
				fprintf(log, "%d %d\n", i, j);
			}
		}
	}
	else if (snake.head.y < snake.tail.y) { // если змея горизонтальна, и голова направлена влево
		fprintf(log, "(4)\n");
		for (int i = snake.tail.x; i <= snake.head.x; ++i) {
			for (int j = snake.tail.y; j >= snake.head.y; --j) {
				field[i][j] = SNAKE;
				fprintf(log, "%d %d\n", i, j);
			}
		}
	}
	
	
	
	
	snake.speed = speed;
}

Field::~Field() {
	for (int i = 0; i < x; ++i) {
		delete [] field[i];
	}
	delete [] field;
}

void Field::print_field(int x, int y) {
	//initscr();
	for (int i = x; i < x + this->x; ++i) {
		for (int j = y; j < y + this->y; ++j) {
			switch(field[i - x][j - y]) {
				case FIELD:
					mvaddch(i, j, ' ');
					break;
				case SNAKE:
					mvaddch(i, j, '*');
					break;
				case FOOD:
					mvaddch(i, j, '+');
					break;
				case FRAME:
					mvaddch(i, j, '0');
					break;
				case NOFRAME:
					mvaddch(i, j, '.');
					break;
			}
		}
	}
	mvaddch(x + snake.head.x, y + snake.head.y, '%');
	refresh();
	//endwin();
}

size_t Field::get_x() { return x; }
size_t Field::get_y() { return y; }


void Field::add_food()  {
	// создаем еду
	Point p;
	while (1) {
		p = {rand() % (this->get_x() - 2) + 1, rand() % (this->get_y() - 2) + 1};
		if (field[p.x][p.y] == FIELD) {
			field[p.x][p.y] = FOOD;
			break;
		}
	}
	
	fprintf(log, "Food generated: (%2d, %2d).\n", p.x, p.y);
	fflush(log);
}


#define VER 0 // вертикаль
#define HOR  1 // горизонталь


int Field::move() {
	noecho();
	curs_set(0);
	keypad(stdscr, true);
	
	int direction_head, direction_tail; // определяем текущее направление движения головы и хвоста змейки. VER(tical) - вертикальное. HOR(izontal) - горизонтальное.
	if (snake.twists.empty()) {
		if (snake.head.x == snake.tail.x) { // горизонтальное
			direction_head = direction_tail = HOR;
		}
		else {
			direction_head = direction_tail = VER;
		}
	}
	else {
		
		// направление головы
		if (snake.head.x == snake.twists.back().x) { // горизонталь
			direction_head = HOR;
		}
		else {
			direction_head = VER;
		}
		
		// направление хвоста
		
		if (snake.tail.x == snake.twists.front().x) {
			direction_tail = HOR;
		}
		else {
			direction_tail = VER;
		}
	}
	
	halfdelay(MAX_SPEED - snake.speed + 1); // переводим в десятые части секунды (требует функция).
	
	size_t old_tail_x = snake.tail.x;
	size_t old_tail_y = snake.tail.y;
	
	field[snake.tail.x][snake.tail.y] = FIELD;
	
	fprintf(log, "DIRECTION_HEAD = %s\n", (direction_head == HOR ? "HOR" : "VER"));
	
	switch(getch()) {
		case KEY_DOWN:
		case 's':
			if (direction_head == HOR) { // горизонталь - поворот
				snake.twists.push(snake.head);
				fprintf(log, "begin\n");
				new_pos(snake.head.x, snake.head.y, 1, 0, x, y, frame, snake.twists);
				fprintf(log, "end\n");
				if (field[snake.head.x][snake.head.y] == FOOD) {
					field[snake.head.x][snake.head.y] = SNAKE;
					field[snake.tail.x][snake.tail.y] = SNAKE;
					add_food();
					return 0;
				}
				if (direction_tail == VER) snake.tail.x -= foo((int)snake.tail.x - (int)snake.twists.front().x);
				else if (direction_tail == HOR) snake.tail.y -= foo((int)snake.tail.y - (int)snake.twists.front().y);
				//if (snake.tail == snake.twists.front()) snake.twists.pop();
			}
			else { // вертикаль - нет поворота - движение вперед
				if (snake.twists.empty()) {
					new_pos(snake.tail.x, snake.tail.y, -foo((int)snake.tail.x - (int)snake.head.x), 0, x, y, frame, snake.twists);
					//snake.tail.x -= foo((int)snake.tail.x - (int)snake.head.x);
					
					new_pos(snake.head.x, snake.head.y, -foo((int)snake.tail.x - (int)snake.head.x), 0, x, y, frame, snake.twists);
					//snake.head.x -= foo((int)snake.tail.x - (int)snake.head.x);
				}
				else {
					new_pos(snake.tail.x, snake.tail.y, -foo((int)snake.tail.x - (int)snake.twists.front().x), 0, x, y, frame, snake.twists);
					new_pos(snake.tail.x, snake.tail.y, 0, -foo((int)snake.tail.y - (int)snake.twists.front().y), x, y, frame, snake.twists);
					//snake.tail.x -= foo((int)snake.tail.x - (int)snake.twists.front().x);
					//snake.tail.y -= foo((int)snake.tail.y - (int)snake.twists.front().y);
					
					
					
					new_pos(snake.head.x, snake.head.y, -foo((int)snake.twists.back().x - (int)snake.head.x), 0, x, y, frame, snake.twists);
					//snake.head.x -= foo((int)snake.twists.back().x - (int)snake.head.x);
				}
			}
			if (snake.tail == snake.twists.front()) snake.twists.pop();
			break;
		
		case KEY_UP:
		case 'w':
			if (direction_head == HOR) {
				snake.twists.push(snake.head);
				new_pos(snake.head.x, snake.head.y, -1, 0, x, y, frame, snake.twists);
				if (field[snake.head.x][snake.head.y] == FOOD) {
					field[snake.head.x][snake.head.y] = SNAKE;
					field[snake.tail.x][snake.tail.y] = SNAKE;
					add_food();
					return 0;
				}
				if (direction_tail == VER) snake.tail.x -= foo((int)snake.tail.x - (int)snake.twists.front().x);
				else if (direction_tail == HOR) snake.tail.y -= foo((int)snake.tail.y - (int)snake.twists.front().y);
				//if (snake.tail == snake.twists.front()) snake.twists.pop();
			}
			else { // вертикаль - нет поворота - движение вперед
				if (snake.twists.empty()) {
					snake.tail.x -= foo((int)snake.tail.x - (int)snake.head.x);
					
					new_pos(snake.head.x, snake.head.y, -foo((int)snake.tail.x - (int)snake.head.x), 0, x, y, frame, snake.twists);
					//snake.head.x -= foo((int)snake.tail.x - (int)snake.head.x);
				}
				else {
					snake.tail.x -= foo((int)snake.tail.x - (int)snake.twists.front().x);
					snake.tail.y -= foo((int)snake.tail.y - (int)snake.twists.front().y);
				
					fprintf(log, "*** (%3d, %3d) ***\n", snake.twists.back().x, snake.twists.back().y);
				
					new_pos(snake.head.x, snake.head.y, -foo((int)snake.twists.back().x - (int)snake.head.x), 0, x, y, frame, snake.twists);
					//snake.head.x -= foo((int)snake.twists.back().x - (int)snake.head.x);
				}
			}
			if (snake.tail == snake.twists.front()) snake.twists.pop();
			break;
			
		case KEY_RIGHT:
		case 'd':
			if (direction_head == VER) {
				snake.twists.push(snake.head);
				new_pos(snake.head.x, snake.head.y, 0, 1, x, y, frame, snake.twists);
				if (field[snake.head.x][snake.head.y] == FOOD) {
					field[snake.head.x][snake.head.y] = SNAKE;
					field[snake.tail.x][snake.tail.y] = SNAKE;
					add_food();
					return 0;
				}
				if (direction_tail == VER) snake.tail.x -= foo((int)snake.tail.x - (int)snake.twists.front().x);
				else if (direction_tail == HOR) snake.tail.y -= foo((int)snake.tail.y - (int)snake.twists.front().y);
				//if (snake.tail == snake.twists.front()) snake.twists.pop();
			}
			else { // горизонталь - нет поворота - движение вперед
				if (snake.twists.empty()) {
					snake.tail.y -= foo((int)snake.tail.y - (int)snake.head.y);
					
					new_pos(snake.head.x, snake.head.y, 0, -foo((int)snake.tail.y - (int)snake.head.y), x, y, frame, snake.twists);
					//snake.head.y -= foo((int)snake.tail.y - (int)snake.head.y);
				}
				else {
					snake.tail.x -= foo((int)snake.tail.x - (int)snake.twists.front().x);
					snake.tail.y -= foo((int)snake.tail.y - (int)snake.twists.front().y);
					
					new_pos(snake.head.x, snake.head.y, 0, -foo((int)snake.twists.back().y - ((int)snake.head.y + 1)), x, y, frame, snake.twists);
					//snake.head.y -= foo((int)snake.twists.back().y - (int)snake.head.y);
				}
			}
			if (snake.tail == snake.twists.front()) snake.twists.pop();
			break;
		
		case KEY_LEFT:
		case 'a':
			if (direction_head == VER) {
				snake.twists.push(snake.head);
				new_pos(snake.head.x, snake.head.y, 0, -1, x, y, frame, snake.twists);
				if (field[snake.head.x][snake.head.y] == FOOD) {
					field[snake.head.x][snake.head.y] = SNAKE;
					field[snake.tail.x][snake.tail.y] = SNAKE;
					add_food();
					return 0;
				}
				if (direction_tail == VER) snake.tail.x -= foo((int)snake.tail.x - (int)snake.twists.front().x);
				else if (direction_tail == HOR) snake.tail.y -= foo((int)snake.tail.y - (int)snake.twists.front().y);
				//if (snake.tail == snake.twists.front()) snake.twists.pop();
			}
			else { // горизонталь - нет поворота - движение вперед
				if (snake.twists.empty()) {
					snake.tail.y -= foo((int)snake.tail.y - (int)snake.head.y);
					
					new_pos(snake.head.x, snake.head.y, 0, -foo((int)snake.tail.y - (int)snake.head.y), x, y, frame, snake.twists);
					//snake.head.y -= foo((int)snake.tail.y - (int)snake.head.y);
				}
				else {
					snake.tail.x -= foo((int)snake.tail.x - (int)snake.twists.front().x);
					snake.tail.y -= foo((int)snake.tail.y - (int)snake.twists.front().y);
					
					fprintf(log, "*** (%3d, %3d) ***\n", snake.twists.back().x, snake.twists.back().y);
					new_pos(snake.head.x, snake.head.y, 0, -foo((int)snake.twists.back().y - ((int)snake.head.y - 1)), x, y, frame, snake.twists);
					//snake.head.y -= foo((int)snake.twists.back().y - (int)snake.head.y);
				}
			}
			if (snake.tail == snake.twists.front()) snake.twists.pop();
			break;
		case ERR:
			
			if (direction_head == VER) {
				if (snake.twists.empty()) {
					snake.tail.x -= foo((int)snake.tail.x - (int)snake.head.x);
					
					new_pos(snake.head.x, snake.head.y, -foo((int)snake.tail.x - (int)snake.head.x), 0, x, y, frame, snake.twists);
					//snake.head.x -= foo((int)snake.tail.x - (int)snake.head.x);
				}
				else {
					snake.tail.x -= foo((int)snake.tail.x - (int)snake.twists.front().x);
					snake.tail.y -= foo((int)snake.tail.y - (int)snake.twists.front().y);
					
					new_pos(snake.head.x, snake.head.y, -foo((int)snake.twists.back().x - (int)snake.head.x), 0, x, y, frame, snake.twists);
					//snake.head.x -= foo((int)snake.twists.back().x - (int)snake.head.x);
				}
				//snake.head.x -= foo((int)snake.head.x - (int)snake.twists.back().x);
				//if (direction_tail == VER) snake.tail.x -= foo((int)snake.tail.x - (int)snake.twists.front().x);
				//else if (direction_tail == HOR) snake.tail.y -= foo((int)snake.tail.y - (int)snake.twists.front().y);
				//if (snake.tail == snake.twists.front()) snake.twists.pop();
			}
			else if (direction_head == HOR) {
				if (snake.twists.empty()) {
					snake.tail.y -= foo((int)snake.tail.y - (int)snake.head.y);
					
					new_pos(snake.head.x, snake.head.y, 0, -foo((int)snake.tail.y - (int)snake.head.y), x, y, frame, snake.twists);
					//snake.head.y -= foo((int)snake.tail.y - (int)snake.head.y);
				}
				else {
					snake.tail.x -= foo((int)snake.tail.x - (int)snake.twists.front().x);
					snake.tail.y -= foo((int)snake.tail.y - (int)snake.twists.front().y);
					
					new_pos(snake.head.x, snake.head.y, 0, -foo((int)snake.twists.back().y - (int)snake.head.y), x, y, frame, snake.twists);
					//snake.head.y -= foo((int)snake.twists.back().y - (int)snake.head.y);
				}
				//snake.head.y -= foo((int)snake.head.y - (int)snake.twists.back().y);
				//if (direction_tail == VER) snake.tail.x -= foo((int)snake.tail.x - (int)snake.twists.front().x);
				//else if (direction_tail == HOR) snake.tail.y -= foo((int)snake.tail.y - (int)snake.twists.front().y);
				//if (snake.tail == snake.twists.front()) snake.twists.pop();
			}
			
			if (field[snake.head.x][snake.head.y] == FOOD) {
					snake.tail.x = old_tail_x;
					snake.tail.y = old_tail_y;
					field[snake.head.x][snake.head.y] = SNAKE;
					field[snake.tail.x][snake.tail.y] = SNAKE;
					add_food();
					return 0;
			}
			
			if (snake.tail == snake.twists.front()) snake.twists.pop();
			break;
		
		default:
			/*в топку*/
			break;
	}
	
	switch (field[snake.head.x][snake.head.y]) {
		case SNAKE:
		case FRAME:
			clear();
			mvprintw(5, 5, "Game Over!\n");
			refresh();
			delay(3);
			fprintf(log, "game over\n"); fflush(log);
			return 1; // ПАМЯТЬ ТЕЧЁТ!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//menu();
			break;
		case FOOD:
			field[snake.head.x][snake.head.y] = SNAKE;
			snake.tail.x = old_tail_x;
			snake.tail.y = old_tail_y;
			field[old_tail_x][old_tail_y] = SNAKE; // ТУТ ТВОРИТСЯ ДИЧЬ, НО ДО СЮДА ДЕЛО ДОЙТИ НЕ ДОЛЖНО
			add_food();
			break;
		case FIELD:
			field[snake.head.x][snake.head.y] = SNAKE;
			break;
		default:
			/*в топку*/
			break;
	}
	
	return 0;
	
}


