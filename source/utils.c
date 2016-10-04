#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/time.h>

void delay (unsigned int mseconds) {
	clock_t goal = mseconds + clock();
	while (goal > clock());
}