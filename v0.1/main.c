
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/time.h>

#include <psp2/ctrl.h>
#include <psp2/touch.h>
#include <psp2/display.h>
#include <psp2/gxm.h>
#include <psp2/types.h>
#include <psp2/moduleinfo.h>
#include <psp2/kernel/processmgr.h>

#include <vita2d.h>

#include "utils.h"
#include "draw.h"


PSP2_MODULE_INFO(0, 0, "vitaAirHockey");

int main()
{
	init_video();

	/* Enable analog stick */
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);

	/* Enable front and (maybe) back touchscreen */
	sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, 1);
	sceTouchSetSamplingState(SCE_TOUCH_PORT_BACK, 1);

	/* FPS counting */
	SceUInt64 cur_micros = 0, delta_micros = 0, last_micros = 0;
	uint32_t frames = 0;
	float fps = 0.0f;

	/* Variables */
/*	int x = SCREEN_W/2 - w/2;
	int y = SCREEN_H/2 - h/2;
*/	int speed = 10;
	
	int r = 35;
//	uint32_t color = RGBA8(255, 0, 0, 255);
	
	/* Mallet1 variables */
	int x1 = 70;
	int y1 = SCREEN_H/2 - r/2;
	int start_x1 = 70;
	
	/* Mallet2 variables */
	int x2 = SCREEN_W - 70;
	int y2 = SCREEN_H/2 - r/2;
	int start_x2 = SCREEN_W - 70;
	
	/* Puck variables */
	int x3 = 180;
	int y3 = SCREEN_H/2 - r/2;
	int puck_is_moving_right = 0;
	int puck_is_moving_left = 0;
	int puck_is_moving_up = 0;
	int puck_is_moving_down = 0;
	int start_x = SCREEN_W/2;
	int start_x3_1 = x3;
	int start_x3_2 = SCREEN_W - 180;
	int start_y = y3;
	
	/* Input variables */
	SceCtrlData pad;
	SceTouchData touch;
	signed char lx, ly, rx, ry;
	
	void delay (unsigned int mseconds) {
		clock_t goal = mseconds + clock();
		while (goal > clock());
	}
	
	/* Game variables */
//	int start_game = 0;
	int m1_points = 0;
	int m2_points = 0;
//	int last_point = 0;

	while (1) {
			
		clear_screen();

		/* Read controls and touchscreen */
		sceCtrlPeekBufferPositive(0, &pad, 1);
		sceTouchPeek(0, &touch, 1);

		if (pad.buttons & SCE_CTRL_START) {
			break;
		}

		/* Draw the center line */
		draw_rectangle (SCREEN_W/2-5, 0, 10, SCREEN_H, GREY);
		
		font_draw_string(10, 10, DARK_GREY, "vitaAirHockey");
		font_draw_stringf(SCREEN_W - 160, 10, GREEN, "FPS: %.2f", fps);
		font_draw_stringf(10, 30, DARK_GREY,
			"(%3d, %3d) (%3d, %3d) (%3d, %3d) size: (%d) speed: %d\n", x1, y1, x2, y2, x3, y3, r, speed);
		font_draw_stringf(10, 50, RED, "%3d -", m1_points);
		font_draw_stringf(60, 50, BLUE, " %3d", m2_points);

		/* Move the mallet 1 */
		if (pad.buttons & SCE_CTRL_UP) {
			y1 -= speed;
		} else 	if (pad.buttons & SCE_CTRL_DOWN) {
			y1 += speed;
		}
		if (pad.buttons & SCE_CTRL_LEFT) {
			x1 -= speed;
		} else 	if (pad.buttons & SCE_CTRL_RIGHT) {
			x1 += speed;
		}
		
		/* Increase/decrease the speed of mallets and puck movement */
		if (pad.buttons & SCE_CTRL_LTRIGGER) {
			delay(5000);
			speed--;
			if (speed < 0) speed = 0;
		} else 	if (pad.buttons & SCE_CTRL_RTRIGGER) {
			delay(5000);
			speed++;
			if (speed > 50) speed = 50;
		}
		
/*		if (pad.buttons & SCE_CTRL_CROSS) {
			color = RGBA8(rand()%255, rand()%255, rand()%255, 255);
		}
*/
		/* Update joystick values */
		lx = (signed char)pad.lx - 128;
		ly = (signed char)pad.ly - 128;
		rx = (signed char)pad.rx - 128;
		ry = (signed char)pad.ry - 128;

		/* Move mallet 1 using the left yoystick */
		if (abs(lx) > 50) {
			x1 += speed * lx/50.0f;
		}
		if (abs(ly) > 50) {
			y1 += speed * ly/50.0f;
		}

		/* Move mallet 2 using the right yoystick */
		if (abs(rx) > 50) {
			x2 += speed * rx/50.0f;
		}
		if (abs(ry) > 50) {
			y2 += speed * ry/50.0f;
		}

		/* Resize using the right yoystick */
/*		if (abs(rx) > 50) {
			w += rx/15.0f;
			if (w < 5) {
				w = 5;
			} else if (w > SCREEN_W) {
				w = SCREEN_W;
			}
		}
		if (abs(ry) > 50) {
			h += ry/15.0f;
			if (h < 5) {
				h = 5;
			} if (h > SCREEN_H) {
				h = SCREEN_H;
			}
		}
*/
		/* Move using the touchscreen! */
		if (touch.reportNum > 0) {
			/* Front touchscreen: 1920x1088 */
			if ((lerp(touch.report[0].x, 1920, SCREEN_W) < SCREEN_W/2) && 
			(lerp(touch.report[0].x, 1920, SCREEN_W) >= x1-100) && (lerp(touch.report[0].x, 1920, SCREEN_W) <= x1+100) &&
			(lerp(touch.report[0].y, 1920, SCREEN_W) >= y1-100) && (lerp(touch.report[0].y, 1920, SCREEN_W) <= y1+100)) {
				x1 = lerp(touch.report[0].x, 1920, SCREEN_W);
				y1 = lerp(touch.report[0].y, 1088, SCREEN_H);
				if ((lerp(touch.report[1].x, 1920, SCREEN_W) > SCREEN_W/2) &&
				(lerp(touch.report[1].x, 1920, SCREEN_W) >= x2-100) && (lerp(touch.report[1].x, 1920, SCREEN_W) <= x2+100) &&
				(lerp(touch.report[1].y, 1920, SCREEN_W) >= y2-100) && (lerp(touch.report[1].y, 1920, SCREEN_W) <= y2+100)) {
					x2 = lerp(touch.report[1].x, 1920, SCREEN_W);
					y2 = lerp(touch.report[1].y, 1088, SCREEN_H);
				}
			}
			else if ((lerp(touch.report[0].x, 1920, SCREEN_W) > SCREEN_W/2) &&
			(lerp(touch.report[0].x, 1920, SCREEN_W) >= x2-100) && (lerp(touch.report[0].x, 1920, SCREEN_W) <= x2+100) &&
			(lerp(touch.report[0].y, 1920, SCREEN_W) >= y2-100) && (lerp(touch.report[0].y, 1920, SCREEN_W) <= y2+100)) {
				x2 = lerp(touch.report[0].x, 1920, SCREEN_W);
				y2 = lerp(touch.report[0].y, 1088, SCREEN_H);
				if ((lerp(touch.report[1].x, 1920, SCREEN_W) < SCREEN_W/2) &&
				(lerp(touch.report[1].x, 1920, SCREEN_W) >= x1-100) && (lerp(touch.report[1].x, 1920, SCREEN_W) <= x1+100) &&
				(lerp(touch.report[1].y, 1920, SCREEN_W) >= y1-100) && (lerp(touch.report[1].y, 1920, SCREEN_W) <= y1+100)) {
					x1 = lerp(touch.report[1].x, 1920, SCREEN_W);
					y1 = lerp(touch.report[1].y, 1088, SCREEN_H);
				}
			}
		}
		
		/* Check left and right collisions */
		if (x1 < r) {
			x1 = r;
		} else if ((x1 + r) > (SCREEN_W/2)) {
			x1 = SCREEN_W/2 - r;
		}

		if ((x2 - r) < (SCREEN_W/2)) {
			x2 = SCREEN_W/2 + r;
		} else if ((x2 + r) > SCREEN_W) {
			x2 = SCREEN_W - r;
		}
		

		/* Check top and bottom collisions */
		if (y1 < r) {
			y1 = r;
		} else if ((y1 + r) > SCREEN_H) {
			y1 = SCREEN_H - r;
		}

		if (y2 < r) {
			y2 = r;
		} else if ((y2 + r) > SCREEN_H) {
			y2 = SCREEN_H - r;
		}
		

		/* Check puck collision and define the movements */
		if (x1 > x3) { // if mallet1 is on the right
			if (((x1-x3) >= 0) && ((x1-x3) <= 70)) {
				if (y1 > y3) { // if mallet1 is on the bottom
					if (((y1-y3) >= 0) && ((y1-y3) <= 70)) {
						puck_is_moving_right=0;
						puck_is_moving_left=1;
						puck_is_moving_up=1;
						puck_is_moving_down=0;
					}
				}
				else if (y1 < y3) { // if mallet1 is on the top
					if (((y3-y1) >= 0) && ((y3-y1) <= 70)) {
						puck_is_moving_right=0;
						puck_is_moving_left=1;
						puck_is_moving_up=0;
						puck_is_moving_down=1;
					}					
				}
				else {
					puck_is_moving_right=0;
					puck_is_moving_left=1;
					puck_is_moving_up=0;
					puck_is_moving_down=0;
				}
			}
		}
		else if (x1 < x3) { //if mallet1 is on the left
			if (((x3-x1) >= 0) && ((x3-x1) <= 70)) { 
				if (y1 > y3) { // if mallet1 is on the bottom
					if (((y1-y3) >= 0) && ((y1-y3) <= 70)) {
						puck_is_moving_right=1;
						puck_is_moving_left=0;
						puck_is_moving_up=1;
						puck_is_moving_down=0;
					}
				}
				else if (y1 < y3) {
					if (((y3-y1) >= 0) && ((y3-y1) <= 70)) { // if mallet1 is on the top
						puck_is_moving_right=1;
						puck_is_moving_left=0;
						puck_is_moving_up=0;
						puck_is_moving_down=1;
					}					
				}
				else {
					if ((puck_is_moving_right == 1) || (puck_is_moving_left == 1)) {
						x3 = x1 + r * 2;
					}
					if ((puck_is_moving_left == 0) && (puck_is_moving_right == 0) && (puck_is_moving_up == 0) && (puck_is_moving_down == 0)) {
						x3 = x1 + r * 2;
					}
						puck_is_moving_right=1;
						puck_is_moving_left=0;
						puck_is_moving_up=0;
						puck_is_moving_down=0;
				}
			}
		}
		else {
			if (y1 > y3) { // if mallet1 is on the bottom
				if (((y1-y3) >= 0) && ((y1-y3) <= 70)) {
					puck_is_moving_right=0;
					puck_is_moving_left=0;
					puck_is_moving_up=1;
					puck_is_moving_down=0;
				}
			}
			else if (y1 < y3) {
				if (((y3-y1) >= 0) && ((y3-y1) <= 70)) { // if mallet1 is on the top
					puck_is_moving_right=0;
					puck_is_moving_left=0;
					puck_is_moving_up=0;
					puck_is_moving_down=1;
				}					
			}
			else {
					puck_is_moving_right=0;
					puck_is_moving_left=0;
					puck_is_moving_up=0;
					puck_is_moving_down=0;
			}
		}

		if (x2 > x3) { // if mallet2 is on the right
			if (((x2-x3) >= 0) && ((x2-x3) <= 70)) {
				if (y2 > y3) { // if mallet2 is on the bottom
					if (((y2-y3) >= 0) && ((y2-y3) <= 70)) {
						puck_is_moving_right=0;
						puck_is_moving_left=1;
						puck_is_moving_up=1;
						puck_is_moving_down=0;
					}
				}
				else if (y2 < y3) { // if mallet2 is on the top
					if (((y3-y2) >= 0) && ((y3-y2) <= 70)) {
						puck_is_moving_right=0;
						puck_is_moving_left=1;
						puck_is_moving_up=0;
						puck_is_moving_down=1;
					}					
				}
				else {
					if ((puck_is_moving_left == 1) || (puck_is_moving_right == 1)) {
						x3 = x2 - r * 2;
					}
					if ((puck_is_moving_left == 0) && (puck_is_moving_right == 0) && (puck_is_moving_up == 0) && (puck_is_moving_down == 0)) {
						x3 = x2 - r * 2;
					}
					puck_is_moving_right=0;
					puck_is_moving_left=1;
					puck_is_moving_up=0;
					puck_is_moving_down=0;
				}
			}
		}
		else if (x2 < x3) { //if mallet2 is on the left
			if (((x3-x2) >= 0) && ((x3-x2) <= 70)) { 
				if (y2 > y3) { // if mallet2 is on the bottom
					if (((y2-y3) >= 0) && ((y2-y3) <= 70)) {
						puck_is_moving_right=1;
						puck_is_moving_left=0;
						puck_is_moving_up=1;
						puck_is_moving_down=0;
					}
				}
				else if (y2 < y3) {
					if (((y3-y2) >= 0) && ((y3-y2) <= 70)) { // if mallet2 is on the top
						puck_is_moving_right=1;
						puck_is_moving_left=0;
						puck_is_moving_up=0;
						puck_is_moving_down=1;
					}					
				}
				else {
						puck_is_moving_right=1;
						puck_is_moving_left=0;
						puck_is_moving_up=0;
						puck_is_moving_down=0;
				}
			}
		}
		else {
			if (y2 > y3) { // if mallet2 is on the bottom
				if (((y2-y3) >= 0) && ((y2-y3) <= 70)) {
					puck_is_moving_right=0;
					puck_is_moving_left=0;
					puck_is_moving_up=1;
					puck_is_moving_down=0;
				}
			}
			else if (y2 < y3) {
				if (((y3-y2) >= 0) && ((y3-y2) <= 70)) { // if mallet2 is on the top
					puck_is_moving_right=0;
					puck_is_moving_left=0;
					puck_is_moving_up=0;
					puck_is_moving_down=1;
				}					
			}
			else {
					puck_is_moving_right=0;
					puck_is_moving_left=0;
					puck_is_moving_up=0;
					puck_is_moving_down=0;
			}
		}
		
		
		if (puck_is_moving_right == 1) {
			if ((x3 + r) < SCREEN_W) {
				x3 += speed;
			}
			else {
				puck_is_moving_right = 0;
				puck_is_moving_left = 1;
			}
		}
		
		if (puck_is_moving_left == 1) {
			if ((x3 - r) > 0) {
				x3 -= speed;
			}
			else {
				puck_is_moving_left = 0;
				puck_is_moving_right = 1;
			}
		}
		
		if (puck_is_moving_up == 1) {
			if ((y3 - r) > (speed)) {
				y3 -= speed;
			}
			else {
				puck_is_moving_up = 0;
				puck_is_moving_down = 1;
			}
		}
		
		if (puck_is_moving_down == 1) {
			if ((y3 + r) <= (SCREEN_H - speed)) {
				y3 += speed;
			}
			else {
				puck_is_moving_down = 0;
				puck_is_moving_up = 1;
			}
		}
		
		if (((x3 + r) <0) || ((x3 - r) > SCREEN_W) || ((y3 + r) < 0) || ((y3 - r) > SCREEN_H)) {
			x3 = start_x;
			y3 = start_y;
			puck_is_moving_up = 0;
			puck_is_moving_down = 0;
			puck_is_moving_left= 0;
			puck_is_moving_right = 0;
		}
		
		if (((x3 + r) >= SCREEN_W-5) && (y3 >= (SCREEN_H/4)) && (y3 <= ((SCREEN_H/4)*3))) {
			m1_points++;
			x3 = start_x3_2;
			y3 = start_y;
			x1 = start_x1;
			y1 = start_y;
			x2 = start_x2;
			y2 = start_y;
			puck_is_moving_up = 0;
			puck_is_moving_down = 0;
			puck_is_moving_left= 0;
			puck_is_moving_right = 0;
		}
		
		if (((x3 - r) <= 5) && (y3 >= (SCREEN_H/4)) && (y3 <= ((SCREEN_H/4)*3))) {
			m2_points++;
			x3 = start_x3_1;
			y3 = start_y;
			x1 = start_x1;
			y1 = start_y;
			x2 = start_x2;
			y2 = start_y;
			puck_is_moving_up = 0;
			puck_is_moving_down = 0;
			puck_is_moving_left= 0;
			puck_is_moving_right = 0;
		}
		
		/* Draw goals */
		draw_rectangle (0, SCREEN_H/4, 5, SCREEN_H/2, GREY); //Left goal
		draw_rectangle (SCREEN_W-5, SCREEN_H/4, 5, SCREEN_H/2, GREY); //Right goal
		
		/* Draw the puck */
		draw_circle(x3, y3, r, BLACK);
		
		/* Draw the mallets */
		draw_circle(x1, y1, r, RED); //Mallet 1 (left)
		draw_circle(x2, y2, r, BLUE); //Mallet 2 (right)

		
		/* Draw a circle */
//		draw_circle(SCREEN_W / 2, SCREEN_H / 2, 50, RGBA8(0,0,255,255));

		/* Calculate FPS */
		cur_micros = sceKernelGetProcessTimeWide();

		if (cur_micros >= (last_micros + 1000000)) {
			delta_micros = cur_micros - last_micros;
			last_micros = cur_micros;
			fps = (frames/(double)delta_micros)*1000000.0f;
			frames = 0;
		}

		swap_buffers();
		sceDisplayWaitVblankStart();
		frames++;
	}

	end_video();
	sceKernelExitProcess(0);
	return 0;
}
