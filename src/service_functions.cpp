#include <cstdio>
#include <dirent.h>
#include <cstdlib>
#include <curses.h>
#include <time.h>
#include <winsock2.h>
#include "header.h"
#include "field.h"

void delay(float sec) {
	char str[256];
	halfdelay(sec / 0.1);
	//raw();
	clock_t end = CLOCKS_PER_SEC * sec + clock();
	while (clock() < end) getch(); // а не залагает ли из за этого места? Эта фича освобождает буфер ввода...
	//cbreak();
	halfdelay(100);
}

int zero_exit() {
	exit(0);
	return 0;
}

int minus_one() {
	return -1;
}

int minus_two() {
	return -2;
}

int set(char * message, int min, int max) { // окно ввода данных. Запрашивает повторный ввод данных, меньших min или больших max
	int res = min - 1;
	echo();
	curs_set(TRUE);
	while (res < min || res > max) {
		clear();
		printw("%s", message);
		printw("Enter the integer number from %d to %d: ", min, max);
		refresh();
		scanw("%d", &res);
	}
	
	curs_set(FALSE);
	noecho();
	return res;
}

void create_dir(char * path) {
	mkdir(path);
	DIR * d = opendir(path);
	if (!d) {
		fprintf(log, "!!! CAN NOT CREATE DIR\n");
		exit(0);
	}
	closedir(d);
}

int single_game_set_frame() {
	create_dir(SETTINGS_PATH);
	int frame = set("Set frame? (0 - no. 1 - yes)\n", 0, 1);
	FILE * f = fopen(SINGLE_GAME_CONFIG("frame"), "w");
	fprintf(f, "%d\n", frame);
	
	fclose(f);
	return -1;
}

int single_game_set_speed() {
	create_dir(SETTINGS_PATH);
	int speed = set("Set speed (0 - min. 10 - max)\n", 0, 10);
	FILE * f = fopen(SINGLE_GAME_CONFIG("speed"), "w");
	fprintf(f, "%d\n", speed);
	
	fclose(f);
	return -1;
}



int multi_game_set_ip() {
	create_dir(SETTINGS_PATH);
	FILE * f = fopen(MULTI_GAME_CONFIG("ip"), "w");
	
	srand(time(NULL));
	
	char ip[256] = "";
	int a, b, c, d;
	a = b = c = d = -1;
	
	echo();
	curs_set(TRUE);
	
	printw("Enter opponent's ip (4 integers from 0 to 255, separated by a dot): ");
	while (scanw("%d.%d.%d.%d", &a, &b, &c, &d) != 4 || a < 0 || a > 255 || b < 0 || b > 255 || c < 0 || c > 255 || d < 0 || d > 255) {
		clear();
		printw("*Example: %d.%d.%d.%d*\n", rand() % 256, rand() % 256, rand() % 256, rand() % 256);
		printw("Enter opponent's ip (4 integers from 0 to 255, separated by a dot): ");
		refresh();
	}
	
	sprintf(ip, "%d.%d.%d.%d", a, b, c, d);
	
	fprintf(f, "%s\n", ip);
	fclose(f);
	
	noecho();
	curs_set(FALSE);
	
	return -1;
}

int multi_game_set_frame() {
	create_dir(SETTINGS_PATH);
	int frame = set("Set frame? (0 - no. 1 - yes)\n", 0, 1);
	FILE * f = fopen(MULTI_GAME_CONFIG("frame"), "w");
	fprintf(f, "%d\n", frame);
	
	fclose(f);
	return -1;
}

int multi_game_set_speed() {
	create_dir(SETTINGS_PATH);
	int speed = set("Set speed (0 - min. 10 - max)\n", 0, 10);
	FILE * f = fopen(MULTI_GAME_CONFIG("speed"), "w");
	fprintf(f, "%d\n", speed);
	
	fclose(f);
	return -1;
}

char * get_ip(char * file_name) {
	FILE * f = fopen(file_name, "r");
	char * ip = (char*)calloc(256, sizeof(char));
	if (!f) {
		f = fopen(file_name, "w");
		clear();
		printw("Enter the ip: ");
		refresh();
		echo();
		scanw("%s", ip);
		noecho();
		fprintf(f, "%s", ip);
		fclose(f);
	}
	else {
		fgets(ip, 256, f);
		fclose(f);
	}
	if (strchr(ip, '\n') != NULL) {
		*(strchr(ip, '\n')) = 0;
	}
	clear();
	refresh();
	return ip;
}

void pexit(char * message) {
	fprintf(log, "@@@\nFatal Error: %s\n", message);
	fprintf(log, "Error: %d\n", WSAGetLastError());
	#ifdef RELEASE
	clear();
	system("pause");
	refresh();
	#endif
	exit(errno);
}

void print(char * arr, size_t x, size_t y, int from_x, int from_y) {
	for (size_t i = 0; i < x; ++i) {
		for (size_t j = 0; j < y; ++j) {
			switch(arr[i*x + j]) {
				case FIELD:
					mvaddch(i + from_x, j + from_y, ' ');
					break;
				case SNAKE:
					mvaddch(i + from_x, j + from_y, '*');
					break;
				case FOOD:
					mvaddch(i + from_x, j + from_y, '+');
					break;
				case FRAME:
					mvaddch(i + from_x, j + from_y, '0');
					break;
				case NOFRAME:
					mvaddch(i + from_x, j + from_y, '.');
					break;
			}
		}
	}
}

