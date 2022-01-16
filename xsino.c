/*
MIT/X Consortium License

© 2021-2022 Carl H. Henriksson <xsino at martial dot cc>

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include <errno.h>
#include <stdio.h>
#include <sys/select.h>
#include <time.h>
#include <X11/Xlib.h>

#define CURSOR_SLEEP_RESOLUTION 12
#define CURSOR_STEP_N 60
#define CURSOR_STEP_SIZE 1
#define I_NANOSLEEP 1
#define ITERATION_N 3

void x_clean(void);
int x_loadcursor(void);
int x_savecursor(void);
int x_setup(void);
int x_shake(int);
int x_sleep(void);
int x_updatecursor(void);

static char *progname;
static Display *d;
static Window rw;
static int h, w, x, xx, y, yy;

void x_clean(void) {
	(void) XCloseDisplay(d);
}

int x_loadcursor(void) {
	if (XWarpPointer(d, None, rw, None, None, None, None, xx, yy) == 0) {
		(void) fprintf(stderr, "%s: Failed to manipulate cursor.\n",
			progname);
		return 1;
	}
	XFlush(d);

	return 0;
}

int x_savecursor(void) {
	Window cr, rr;
	int rx, ry, wx, wy;
	unsigned int mr;
	
	if (XQueryPointer(d, rw, &rr, &cr, &rx, &ry, &wx, &wy, &mr) == 0) {
		(void) fprintf(stderr, "%s: Failed to get cursor data.\n",
			progname);
		return 1;
	}
	xx = wx;
	yy = wy;

	return 0;
}

int x_setup(void) {
	XWindowAttributes a;

	d = XOpenDisplay(NULL);
	if (d == NULL) {
		(void) fprintf(stderr, "%s: Failed to connect to X server\n",
			progname);
		return 1;
	}

	rw = XRootWindow(d, 0);
	if (XGetWindowAttributes(d, rw, &a) == 0) {
		(void) fprintf(stderr, "%s: Failed to get window attributes.\n",
			progname);
		return 2;
	}

	w = a.width;
	h = a.height;

	return 0;
}

int x_shake(int vertical) {
	int ofs, xc, yc;
	int in, sn, sr, ss;
	size_t i;

	in = ITERATION_N;
	sn = CURSOR_STEP_N;
	sr = CURSOR_SLEEP_RESOLUTION;
	ss = CURSOR_STEP_SIZE;

	x = xc = w / 2;
	y = yc = h / 2;
	while (in--)
		for (i = 0, ofs = 0; i < sn * 4; i++) {
			if (i > sn && i < sn * 3)
				ofs += ss;
			else
				ofs -= ss;

			if (vertical == 0)
				x = xc + ofs;
			else
				y = yc + ofs;
			
			x_updatecursor();
			if (i % sr == 0)
				x_sleep();
		}

	return 0;
}

int x_sleep(void) {
	struct timespec t;

	t.tv_sec = 0;
	t.tv_nsec = I_NANOSLEEP;

	errno = 0;
	if (nanosleep(&t, NULL) != 0) {
		perror("nanosleep failed");
		return 1;
	}

	return 0;
}

int x_updatecursor(void) {
	if (XWarpPointer(d, None, rw, None, None, None, None, x, y) == 0) {
		(void) fprintf(stderr, "%s: Failed to manipulate cursor.\n",
			progname);
		return 1;
	}
	XFlush(d);

	return 0;
}

int main(int argc, char *argv[]) {
	int horizontal;

	progname = argv[0];

	if (argc != 2) {
		(void) fprintf(stderr, "%s: Invalid argument amount: %d\n",
			progname, argc);
		return 1;
	}

	if (argv[1][0] != '-') {
		(void) fprintf(stderr, "%s: Invalid argument format: %s\n",
			progname, argv[1]);
		return 2;
	}

	switch (argv[1][1]) {
	case 'n':
		horizontal = 0;
		break;
	case 'v':
		(void) fprintf(stderr, "%s-%s\n", progname, VERSION);
		return 0;
	case 'y':
		horizontal = 1;
		break;
	default:
		(void) fprintf(stderr, "%s: Invalid argument: %s\n",
			progname, argv[1]);
		return 3;
	}

	if (x_setup() != 0)
		return 4;

	if (x_savecursor() != 0)
		return 5;

	if (x_shake(horizontal) != 0)
		return 6;

	if (x_loadcursor() != 0)
		return 7;

	x_clean();

	return 0;
}
