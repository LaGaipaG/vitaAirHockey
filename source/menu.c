#include <stdio.h>
#include <stdlib.h>

#include <psp2/ctrl.h>
#include <psp2/touch.h>
#include <psp2/display.h>
#include <psp2/gxm.h>
#include <psp2/types.h>
#include <psp2/moduleinfo.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/ctrl.h>
#include <vita2d.h>

#include "draw.h"
#include "utils.h"

const char *MAIN_MENU[3] = {"Single player(WIP)", "Multiplayer", "Settings(WIP)"};

int i = 0;

SceCtrlData pad;

void main_menu () {
	
	vita2d_set_clear_color(BLACK);
/*
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
	SceCtrlData pad;
*/
	int j = 0;

	for (i=0; i<=2; i++){
		if (i==1)font_draw_stringf(SCREEN_W/3, SCREEN_H/3 + j, WHITE, "%s\n", MAIN_MENU[i]);
		else font_draw_stringf(SCREEN_W/3, SCREEN_H/3 + j, GREY, "%s\n", MAIN_MENU[i]);
		j = j + 30;
	}
	
	font_draw_stringf(SCREEN_W/3-20, SCREEN_H/2 + j+30, WHITE, "Press X or O to begin");
	
	
/*		if (state >= 3) state = 0;
		if (state <= -1) state = 2;

		for (i=0; i<=2; i++) {
			if (i == state){
				font_draw_stringf(SCREEN_W/3, SCREEN_H/3 + j, WHITE, "%s\n", MAIN_MENU[i]);
			}
			else {
				font_draw_stringf(SCREEN_W/3, SCREEN_H/3 + j, GREY, "%s\n", MAIN_MENU[i]);
			}
			j = j + 30;
		}
		j = 0;
*/
//		sceCtrlPeekBufferPositive(0, &pad, 1);
/*		
		if (pad.buttons & SCE_CTRL_DOWN) {
			state = state+1;
			delay (150000);
		}
		else if (pad.buttons & SCE_CTRL_UP) {
			state = state-1;
			delay (150000);
		}
*///		if (pad.buttons & SCE_CTRL_CROSS) break;
//		i = state;
		
}


