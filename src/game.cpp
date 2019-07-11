#include <cstdio>
#include <curses.h>
#include "field.h"
#include "header.h"



#define X 20
#define Y 40


#define START_LEN 3
#define FRAME_CHANGE_IT 1 // добавить выбор рамки

int single_game() {
	//	initscr();
	fprintf(log, "single_game...\n");
	Field field = Field(X, Y, FRAME_CHANGE_IT, START_LEN, 10);
	fprintf(log, "Object Field field created\n");
	clear();
	printw("Single Game\n");
	getch();
	clear();
	mvprintw(0, 0, "Press Any Key To Play\n");
	field.print_field(3, 3);
	getch();
	field.add_food();
	mvprintw(0, 0, "                      \n");
	refresh();
	
	while (!field.move()) {
		field.print_field(3, 3);
	}
	fprintf(log, "game over (while break)\n"); fflush(log);
//	endwin();
	return -2;
}

int multi_game() {
//	initscr();
	clear();
	printw("Multi Game\n");
	getch();
//	endwin();
	return -2;
}


