#include <cstdio>
#include <curses.h>
#include <cstdlib>
#include "header.h"


#define GENERAL_MENU "Snake v0.1\n"\
			 "> START\n"\
			 "  HELP\n"\
			 "  EXIT\n"

#define GENERAL_NUM_OF_ROWS 3 // количество выборных пунктов в GENERAL_MENU


#define START_MENU "Snake v0.1\n"\
				   "> Single Player\n"\
				   "  Multiplayer\n"

#define START_NUM_OF_ROWS 2


#define HELP_MENU "Snake v0.1\n"\
				  "Pomogi sebe sam\n"


int choice(char * choice_menu, int y_start, int num_of_rows) {
	clear();
	printw("%s", choice_menu);
	refresh();
	
	int x = 0, y = y_start;
	int key;
	while ((key = getch()) != '\n' && key != '\b') {
		//mvaddch(10, 10, '0' + key);
		switch(key) {
			case KEY_UP:
				mvaddch(y, x, ' ');
				refresh();
				--y;
				if (y == 0) y = num_of_rows;
				
				mvaddch(y, x, '>');
				refresh();
				break;
			case KEY_DOWN:
				mvaddch(y, x, ' ');
				refresh();
				++y;
				if (y == num_of_rows + 1) {
					y = 1;
				}
				
				mvaddch(y, x, '>');
				refresh();
				break;
		}
	}
	return (key == '\n') ? y : -1;
}

void menu() {
	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, true);
	
	while (1) {
		int y = choice(GENERAL_MENU, 1, GENERAL_NUM_OF_ROWS);
		switch(y) {
			case -1:
				break;
			case 1: { // START
				int y1 = choice(START_MENU, 1, START_NUM_OF_ROWS);
				switch(y1) {
					case -1:
						break;
					case 1:
						single_game();
						break;
					case 2:
						multi_game();
						break;
					default: {
						clear();
						printw("Error: menu -> switch -> case 1 -> switch -> default\n");
						getch();
						endwin();
						exit(0);
						break;
					}
				}
				break;
			}
			
			case 2: { // HELP
				clear();
				printw("%s", HELP_MENU);
				refresh();
				halfdelay(50); // UP IT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				getch();	
				break;
			}
			case 3: {
				endwin();
				exit(0);
				break;
			}
				
			default: {
				clear();
				fprintf(log, "Error: menu -> switch -> default\n");
				getch();
				endwin();
				exit(0);
				break;
			}
		}
	}
	
	endwin();
}



