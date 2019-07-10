#include <cstdio>
#include <cstdlib>
#include <curses.h>
#include <time.h>
#include "header.h"

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


