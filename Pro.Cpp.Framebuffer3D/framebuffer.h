// Framebuffer for 1920x1080@32

typedef struct {
	unsigned char *FrameBuffer;
	struct fb_fix_screeninfo FixedInfo; //  MemLength = FixedInfo.smem_len
	struct fb_var_screeninfo OrigVarInfo; // W:H:Bits = OrigVarInfo.xres : OrigVarInfo.yres : OrigVarInfo.bits_per_pixel
	int FrameBufferFD;
	int ConsoleFD, OriginalVT;
	struct termios con_attr;
} graphics_attribution;

static graphics_attribution vgattr, *_gattr;

int enter_graphics() {
	graphics_attribution *gattr=&vgattr;
	if ((gattr->FrameBufferFD = open("/dev/fb0", O_RDWR)) == -1)
		return 0;
	ioctl(gattr->FrameBufferFD, FBIOGET_FSCREENINFO, &gattr->FixedInfo);
	ioctl(gattr->FrameBufferFD, FBIOGET_VSCREENINFO, &gattr->OrigVarInfo);

	gattr->FrameBuffer = (unsigned char *)mmap(0, gattr->FixedInfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, gattr->FrameBufferFD, 0);
	gattr->ConsoleFD = open("/dev/tty0", O_WRONLY, 0);
	static int vtnumber;
	ioctl(gattr->ConsoleFD, VT_OPENQRY, &vtnumber);
	close(gattr->ConsoleFD);

	static char ttystr[16];
	sprintf(ttystr, "/dev/tty%d", vtnumber);
	gattr->ConsoleFD = open(ttystr, O_RDWR | O_NONBLOCK, 0);

	static struct vt_stat vts;
	if (ioctl(gattr->ConsoleFD, VT_GETSTATE, &vts) == 0)
		gattr->OriginalVT = vts.v_active;

	int ttyfd = open("/dev/tty", O_RDWR);
	ioctl(ttyfd, TIOCNOTTY, 0);
	close(ttyfd);

	static struct vt_mode vt;
	ioctl(gattr->ConsoleFD, VT_ACTIVATE, vtnumber);
	ioctl(gattr->ConsoleFD, VT_WAITACTIVE, vtnumber);
	ioctl(gattr->ConsoleFD, VT_GETMODE, &vt);

	vt.mode = VT_PROCESS;
	vt.acqsig = vt.relsig = 10;

	ioctl(gattr->ConsoleFD, VT_SETMODE, &vt);
	ioctl(gattr->ConsoleFD, KDSETMODE, KD_GRAPHICS);

	tcgetattr(gattr->ConsoleFD, &gattr->con_attr);
	gattr->con_attr.c_lflag ^= (ECHO | ICANON);
	tcsetattr(gattr->ConsoleFD, TCSAFLUSH, &gattr->con_attr);

	_gattr=gattr;
	return 1;
}

void exit_graphics() {
	graphics_attribution *gattr=&vgattr;
	struct vt_mode VT;
	ioctl(gattr->FrameBufferFD, FBIOPUT_VSCREENINFO, &gattr->OrigVarInfo);
	munmap(gattr->FrameBuffer, gattr->FixedInfo.smem_len);
	close(gattr->FrameBufferFD);

	ioctl(gattr->ConsoleFD, KDSETMODE, KD_TEXT);
	ioctl(gattr->ConsoleFD, VT_GETMODE, &VT);
	VT.mode = VT_AUTO;
	ioctl(gattr->ConsoleFD, VT_SETMODE, &VT);
	ioctl(gattr->ConsoleFD, VT_ACTIVATE, gattr->OriginalVT);

	gattr->con_attr.c_lflag ^= (ECHO | ICANON);
	tcsetattr(gattr->ConsoleFD, TCSAFLUSH, &gattr->con_attr);
	close(gattr->ConsoleFD);
}

int read_keypress() {
	int key;
	if (read(_gattr->ConsoleFD, &key, 4)>0)
		return key;
	return -1;
}

#define VRAM (_gattr->FrameBuffer)
#define MENLEN (_gattr->FixedInfo.smem_len)

#include "draw.h"
#include "gl3d.h"