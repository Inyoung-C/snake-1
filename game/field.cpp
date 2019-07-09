#include <curses.h>
#include <cstdlib>
#include <ctime>
#include "field.h"
#include "header.h"

bool Point::operator==(Point p) {
	return this->x == p.x && this->y == p.y;
}


int foo(int a) { // ну а как еще назвать это чудо?
	if (a > 0) return 1;
	if (a < 0) return -1;
	if (a = 0) return 0;
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
			if (frame) {
				if (i == 0 || i + 1 == x || j == 0 || j + 1 == y) {
					field[i][j] = FRAME;
				}
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
	for (int i = 0; i < y; ++i) {
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
	
	fprintf(log, "Food generated: head = (%d, %d).\n", p.x, p.y);
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
	
	switch(getch()) {
		case KEY_DOWN:
		case 's':
			if (direction_head == HOR) { // горизонталь - поворот
				snake.twists.push(snake.head);
				snake.head.x++;
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
					snake.head.x -= foo((int)snake.tail.x - (int)snake.head.x);
				}
				else {
					snake.tail.x -= foo((int)snake.tail.x - (int)snake.twists.front().x);
					snake.tail.y -= foo((int)snake.tail.y - (int)snake.twists.front().y);
					snake.head.x -= foo((int)snake.twists.back().x - (int)snake.head.x);
				}
			}
			if (snake.tail == snake.twists.front()) snake.twists.pop();
			break;
		
		case KEY_UP:
		case 'w':
			if (direction_head == HOR) {
				snake.twists.push(snake.head);
				snake.head.x--;
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
					snake.head.x -= foo((int)snake.tail.x - (int)snake.head.x);
				}
				else {
					snake.tail.x -= foo((int)snake.tail.x - (int)snake.twists.front().x);
					snake.tail.y -= foo((int)snake.tail.y - (int)snake.twists.front().y);
					snake.head.x -= foo((int)snake.twists.back().x - (int)snake.head.x);
				}
			}
			if (snake.tail == snake.twists.front()) snake.twists.pop();
			break;
			
		case KEY_RIGHT:
		case 'd':
			if (direction_head == VER) {
				snake.twists.push(snake.head);
				snake.head.y++;
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
					snake.head.y -= foo((int)snake.tail.y - (int)snake.head.y);
				}
				else {
					snake.tail.x -= foo((int)snake.tail.x - (int)snake.twists.front().x);
					snake.tail.y -= foo((int)snake.tail.y - (int)snake.twists.front().y);
					snake.head.y -= foo((int)snake.twists.back().y - (int)snake.head.y);
				}
			}
			if (snake.tail == snake.twists.front()) snake.twists.pop();
			break;
		
		case KEY_LEFT:
		case 'a':
			if (direction_head == VER) {
				snake.twists.push(snake.head);
				snake.head.y--;
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
					snake.head.y -= foo((int)snake.tail.y - (int)snake.head.y);
				}
				else {
					snake.tail.x -= foo((int)snake.tail.x - (int)snake.twists.front().x);
					snake.tail.y -= foo((int)snake.tail.y - (int)snake.twists.front().y);
					snake.head.y -= foo((int)snake.twists.back().y - (int)snake.head.y);
				}
			}
			if (snake.tail == snake.twists.front()) snake.twists.pop();
			break;
		case ERR:
			
			if (direction_head == VER) {
				if (snake.twists.empty()) {
					snake.tail.x -= foo((int)snake.tail.x - (int)snake.head.x);
					snake.head.x -= foo((int)snake.tail.x - (int)snake.head.x);
				}
				else {
					snake.tail.x -= foo((int)snake.tail.x - (int)snake.twists.front().x);
					snake.tail.y -= foo((int)snake.tail.y - (int)snake.twists.front().y);
					snake.head.x -= foo((int)snake.twists.back().x - (int)snake.head.x);
				}
				//snake.head.x -= foo((int)snake.head.x - (int)snake.twists.back().x);
				//if (direction_tail == VER) snake.tail.x -= foo((int)snake.tail.x - (int)snake.twists.front().x);
				//else if (direction_tail == HOR) snake.tail.y -= foo((int)snake.tail.y - (int)snake.twists.front().y);
				//if (snake.tail == snake.twists.front()) snake.twists.pop();
			}
			else if (direction_head == HOR) {
				if (snake.twists.empty()) {
					snake.tail.y -= foo((int)snake.tail.y - (int)snake.head.y);
					snake.head.y -= foo((int)snake.tail.y - (int)snake.head.y);
				}
				else {
					snake.tail.x -= foo((int)snake.tail.x - (int)snake.twists.front().x);
					snake.tail.y -= foo((int)snake.tail.y - (int)snake.twists.front().y);
					snake.head.y -= foo((int)snake.twists.back().y - (int)snake.head.y);
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
			return 1; // ПАМЯТЬ ТЕЧЁТ!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! (640 килобайт - не в счёт)
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


