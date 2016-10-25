#include <stdio.h>
#include <xcb/xcb.h>

int main () {
	int i, screenNum;
	xcb_connection_t *connection = xcb_connect (NULL, &screenNum);
	const xcb_setup_t *setup = xcb_get_setup (connection);
	xcb_screen_iterator_t iter = xcb_setup_roots_iterator (setup);  
	for (i = 0; i < screenNum; ++i)
		xcb_screen_next(&iter);
	xcb_screen_t *screen = iter.data;
	printf ("Informations of screen %d:\n", screen->root);
	printf ("  width.........: %d\n", screen->width_in_pixels);
	printf ("  height........: %d\n", screen->height_in_pixels);
	printf ("  white pixel...: %d\n", screen->white_pixel);
	printf ("  black pixel...: %d\n", screen->black_pixel);
	return 0;
}
