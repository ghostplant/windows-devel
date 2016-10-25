#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <linux/vt.h>
#include <linux/kd.h>
#include <sys/mman.h>
#include <asm/ioctls.h>
#include <termios.h>

#include "framebuffer.h"

int main() {
	if (!enter_graphics())
		return 1;

	float ang=0;
	lookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);
	perspective(100, 1, 0.1, 500);
	int i;
	for (i=0;i<400;i++) {
		clear();
		loadIdentity(object);
		scale(2,2,2);
		translate(0.2,0.2,0);
		rotate(ang+=0.05,0,0,1);
		prepareScene();
	
		gl_point a=realPosition(0,0.0,0);
		gl_point b=realPosition(0,0.5,0);
		gl_point c=realPosition(0.5,0,0);
	
		draw_line(a.x,a.y,b.x,b.y,0xff0000);
		draw_line(a.x,a.y,c.x,c.y,0xff0000);
		draw_line(c.x,c.y,b.x,b.y,0xff0000);
		flood_fill((a.x+b.x+c.x)/3,(a.y+b.y+c.y)/3,0xff0000);
		flush();
		usleep(10000);
	}
	clear();
	draw_text(500,400,"Hello  World, this is ghostplant!\n\nPress any key to continue..",0xff);
	draw_circle(700,700,100,0xffff00,30);
	flush();

	while (read_keypress()==-1);
	exit_graphics();
	return 0;
}
