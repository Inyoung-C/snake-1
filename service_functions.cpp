#include <cstdio>
#include <cstdlib>
#include <time.h>
#include "header.h"

void delay(float sec) {
	clock_t end = CLOCKS_PER_SEC * sec + clock();
	while (clock() < end);
}
