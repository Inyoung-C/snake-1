#include <cstdio>
#include <curses.h>
#include <cstdlib>
#include "header.h"
#include "menu.h"

char * GENERAL_MENU = "Snake v0.1\n"\
		 			  ">  START\n"\
	 				  "   HELP\n"\
					  "   EXIT\n";
#define GENERAL_NUM_OF_ROWS 3 // количество выборных пунктов в GENERAL_MENU


char * START_MENU = "Snake v0.1\n"\
				    "> Single Player\n"\
				    "  Multiplayer\n";
#define START_NUM_OF_ROWS 2


char * HELP_MENU = "Snake v0.1\n"\
				   "Pomogi sebe sam\n";
#define HELP_NUM_OF_ROWS 0

char * EXIT_MENU = "Snake v0.1\n"\
				   "Are you sure?\n"\
				   ">  Yes\n"\
				   "   No\n";
#define EXIT_NUM_OF_ROWS 2

char * SINGLEPLAYER_MENU = "Snake v0.1\n"\
						   ">  Play\n"\
						   "   Settings\n";
#define SINGLEPLAYER_NUM_OF_ROWS 2

char * MULTIPLAYER_MENU = "Snake v0.1\n"\
						  ">  Play\n"\
						  "   Settings\n";
#define MULTIPLAYER_NUM_OF_ROWS 2

char * SINGLEPLAYER_SETTINGS_MENU = "Snake v0.1\n"\
									">  Frame\n";
#define SINGLEPLAYER_SETTINGS_NUM_OF_ROWS 1

char * MULTIPLAYER_SETTINGS_MENU = "Snake v0.1\n"\
								   ">  Change Opponent's IP\n"
								   "   Frame\n";
#define MULTIPLAYER_SETTINGS_NUM_OF_ROWS 2


int choice(const Tree * panel) {
	clear();
	if (panel->content != nullptr)
		printw("%s", panel->content);
	refresh();
	
	int x = 0, y = panel->begin;
	int key;
	
	if (panel->content == nullptr) {
		return panel->func();
	}
	
	if (panel->num == 0) {
		getch();
		return -1;
	}
	
	while ((key = getch()) != '\n' && key != '\b') {
		//mvaddch(10, 10, '0' + key);
		switch(key) {
			case KEY_UP:
				mvaddch(y, x, ' ');
				refresh();
				--y;
				if (y == panel->begin - 1) y = panel->num + panel->begin - 1;
				
				mvaddch(y, x, '>');
				refresh();
				break;
			case KEY_DOWN:
				mvaddch(y, x, ' ');
				refresh();
				++y;
				if (y == panel->num + panel->begin) {
					y = panel->begin;
				}
				
				mvaddch(y, x, '>');
				refresh();
				break;
		}
	}
	return (key == '\n') ? y : -1;
}

Tree initPanel(type content, int begin, size_t num, Tree * branchs, Tree * parrent, int (*func)()) {
	Tree panel;
	panel.content = content;
	panel.begin   = begin;
	panel.num     = num;
	panel.branchs = branchs;
	panel.parrent = parrent;
	panel.func    = func;
	return panel;
}

Tree initMenu() {
	Tree menu;
	menu = initPanel(GENERAL_MENU, 1, GENERAL_NUM_OF_ROWS, new Tree[GENERAL_NUM_OF_ROWS], nullptr, nullptr);
	/*menu.content = GENERAL_MENU;
	menu.begin   = 1; 
	menu.num     = GENERAL_NUM_OF_ROWS;
	menu.branchs = new Tree[menu.num];
	menu.parrent = nullptr;*/
	
		Tree & start_menu = menu.branchs[0];
			start_menu = initPanel(START_MENU, 1, START_NUM_OF_ROWS, new Tree[START_NUM_OF_ROWS], &menu, nullptr);
			/*start_menu.content = START_MENU;
			start_menu.begin   = 1;
			start_menu.num     = START_NUM_OF_ROWS;
			start_menu.branchs = new Tree[start_menu.num];
			start_menu.parrent = &menu;*/
			
				Tree & singleplayer_menu = start_menu.branchs[0];
					singleplayer_menu = initPanel(SINGLEPLAYER_MENU, 1, SINGLEPLAYER_NUM_OF_ROWS, new Tree[SINGLEPLAYER_NUM_OF_ROWS], &start_menu, nullptr);
					
						Tree & singleplayer_play          = singleplayer_menu.branchs[0];
							singleplayer_play = initPanel(nullptr, -1, 0, nullptr, &singleplayer_menu, single_game);
						
						Tree & singleplayer_settings_menu = singleplayer_menu.branchs[1];
							singleplayer_settings_menu = initPanel(SINGLEPLAYER_SETTINGS_MENU, 1, SINGLEPLAYER_SETTINGS_NUM_OF_ROWS, new Tree[SINGLEPLAYER_SETTINGS_NUM_OF_ROWS], &singleplayer_menu, nullptr);
				
				
				Tree & multiplayer_menu  = start_menu.branchs[1];
					multiplayer_menu = initPanel(MULTIPLAYER_MENU, 1, MULTIPLAYER_NUM_OF_ROWS, new Tree[MULTIPLAYER_NUM_OF_ROWS], &start_menu, nullptr);
					
						Tree & multiplayer_play = multiplayer_menu.branchs[0];
							multiplayer_play = initPanel(nullptr, -1, 0, nullptr, &multiplayer_menu, multi_game);
						
						Tree & multiplayer_settings_menu = multiplayer_menu.branchs[1];
							multiplayer_settings_menu = initPanel(MULTIPLAYER_SETTINGS_MENU, 1, MULTIPLAYER_SETTINGS_NUM_OF_ROWS, new Tree[MULTIPLAYER_SETTINGS_NUM_OF_ROWS], &multiplayer_menu, nullptr);
		
		Tree & help_menu  = menu.branchs[1];
			help_menu = initPanel(HELP_MENU, -1, HELP_NUM_OF_ROWS, nullptr, &menu, nullptr);
			
		Tree & exit_menu  = menu.branchs[2];
			exit_menu = initPanel(EXIT_MENU, 2, EXIT_NUM_OF_ROWS, new Tree[EXIT_NUM_OF_ROWS], &menu, nullptr);
			
				Tree & exit_yes = exit_menu.branchs[0];
					exit_yes = initPanel(nullptr, -1, 0, nullptr, &exit_menu, zero_exit);
					
				Tree & exit_no  = exit_menu.branchs[1];
					exit_no = initPanel(nullptr, -1, 0, nullptr, &exit_menu, minus_two);
			
			
	
	return menu;
	
}

void menu() {
	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, true);
	
	Tree menu = initMenu();
	
	const Tree * current_panel = &menu;
	
	while (1) {
		int y = choice(current_panel);
		switch(y) {
			case -2:
				current_panel = &menu;
				break;
			case -1:
				if (current_panel->parrent != nullptr) {
					current_panel = current_panel->parrent;
				}
				break;
			default: {
				current_panel = &current_panel->branchs[y - current_panel->begin];
				break;
			}
		}
	}
	
	endwin();
}


/*int choice(char * choice_menu, int y_start, int num_of_rows) { // old version
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
}*/

/*void menu() { // old_version
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
}*/



