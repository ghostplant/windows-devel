#include <math.h>

float project[16], object[16], mixer[16];

float invsqrt(float x){
    float xhalf=0.5f*x;
    long i=*(long*)&x;
    i=0x5f3759df - (i>>1);
    x=*(float *)&i;
    x=x*(1.5f-xhalf*x*x);
    return x;
}

void multiply(float *b, float *a, float *c) {
	float A = b[0], B = b[1], t = b[2], u = b[3], v = b[4], w = b[5], x = b[6], y = b[7], z = b[8], C = b[9], D = b[10], E = b[11], q = b[12], F = b[13], G = b[14], I = b[15];
	float d = a[0], e = a[1], g = a[2], f = a[3], h = a[4], i = a[5], j = a[6], k = a[7], l = a[8], o = a[9], m = a[10], n = a[11], p = a[12], r = a[13], s = a[14], H = a[15];
	c[0] = A * d + B * h + t * l + u * p;
	c[1] = A * e + B * i + t * o + u * r;
	c[2] = A * g + B * j + t * m + u * s;
	c[3] = A * f + B * k + t * n + u * H;
	c[4] = v * d + w * h + x * l + y * p;
	c[5] = v * e + w * i + x * o + y * r;
	c[6] = v * g + w * j + x * m + y * s;
	c[7] = v * f + w * k + x * n + y * H;
	c[8] = z * d + C * h + D * l + E * p;
	c[9] = z * e + C * i + D * o + E * r;
	c[10] = z * g + C * j + D * m + E * s;
	c[11] = z * f + C * k + D * n + E * H;
	c[12] = q * d + F * h + G * l + I * p;
	c[13] = q * e + F * i + G * o + I * r;
	c[14] = q * g + F * j + G * m + I * s;
	c[15] = q * f + F * k + G * n + I * H;
}

void ortho(float left, float right, float bottom, float top, float near, float far) {
	float h = right - left, i = top - bottom, j = far - near;
	float mat4[] = {2/h, 0, 0, -(left+right)/h,
		0, 2/i, 0, -(top+bottom)/i,
		0, 0, -2/j, -(far+near)/j, 0, 0, 0, 1};
	multiply(mat4, project, project);
}

void frustum(float left, float right, float bottom, float top, float near, float far) {
	float h = right - left, i = top - bottom, j = far - near;
	float mat4[] = {near*2/h, 0, (right+left)/h, 0,
		0, near*2/i, (top+bottom)/i, 0,
		0, 0, -(far+near)/j, -(far*near*2)/j, 0, 0, -1, 0};
	multiply(mat4, project, project);
}

void perspective(float fov,float aspect,float near,float far) {
	fov = near * tan(fov * 8.72664626e-3f);
	aspect *= fov;
	frustum(-aspect, aspect, -fov, fov, near, far);
}

void lookAt(float ex,float ey,float ez, float tx, float ty, float tz, float hx, float hy, float hz) {
	float len, Zx = ex-tx, Zy = ey-ty, Zz = ez-tz;
	len = invsqrt(Zx*Zx+Zy*Zy+Zz*Zz);
	Zx*=len, Zy*=len, Zz*=len;
	float Xx = hy*Zz-hz*Zy, Xy = hz*Zx-hx*Zz, Xz = hx*Zy-hy*Zx;
	len = invsqrt(Xx*Xx+Xy*Xy+Xz*Xz);
	Xx*=len, Xy*=len, Xz*=len;
	float Yx = Zy*Xz-Zz*Xy, Yy = Zz*Xx-Zx*Xz, Yz = Zx*Xy-Zy*Xx;
	len = invsqrt(Yx*Yx+Yy*Yy+Yz*Yz);
	Yx*=len, Yy*=len, Yz*=len;
	float mat4[] = {Xx, Xy, Xz, -(Xx*ex+Xy*ey+Xz*ez),
		Yx, Yy, Yz, -(Yx*ex+Yy*ey+Yz*ez),
		Zx, Zy, Zz, -(Zx*ex+Zy*ey+Zz*ez), 0, 0, 0, 1};
	memcpy(project,mat4,sizeof(mat4));
}

float prepareScene() {
	multiply(project, object, mixer);
}

void loadIdentity() {
	memset(object,0,sizeof(object));
	object[0] = object[5] = object[10] = object[15] = 1;
}

void translate(float tx,float ty,float tz) {
	object[3] += tx;
	object[7] += ty;
	object[11] += tz;
}

void scale(float sx,float sy,float sz) {
	object[0] *= sx, object[1] *= sx, object[2] *= sx, object[3] *= sx;
	object[4] *= sy, object[5] *= sy, object[6] *= sy, object[7] *= sy;
	object[8] *= sz, object[9] *= sz, object[10] *= sz, object[11] *= sz;
}

void rotate(float rad, float ax,float ay,float az) {
	float len=invsqrt(ax*ax+ay*ay+az*az);
	ax*=len, ay*=len, az*=len;
	float s=sin(rad), c=cos(rad), t=1-c;
	float xyt=ax*ay*t, yzt=ay*az*t, xzt=ax*az*t, xs=ax*s, ys=ay*s, zs=az*s;
	float mat4[]={(ax*ax*t+c), (xyt-zs), (xzt+ys), 0,
		(xyt+zs), (ay*ay*t+c), (yzt-xs), 0,
		(xzt-ys), (yzt+xs), (az*az*t+c), 0, 0,0,0,1 };
	multiply(mat4, object, object);
}

typedef struct {
	int x,y;
	double deep;
} gl_point;

gl_point realPosition(float x,float y,float z) {
	gl_point ret;
	float k = 1/(mixer[12]*x+mixer[13]*y+mixer[14]*z+mixer[15]);
	ret.y = ((mixer[0]*x+mixer[1]*y+mixer[2]*z+mixer[3])*k+1)*960;
	ret.x = ((mixer[4]*x+mixer[5]*y+mixer[6]*z+mixer[7]) *k+1)*540;
	ret.deep = (mixer[8]*x+mixer[9]*y+mixer[10]*z+mixer[11])*k;
	return ret;
}

void flood_fill(int x,int y,int c) {
	if (inner(x,y)) {
		AT(x,y)=c;
		if (inner(x-1,y) && AT(x-1,y)!=c)
			flood_fill(x-1,y,c);
		if (inner(x+1,y) && AT(x+1,y)!=c)
			flood_fill(x+1,y,c);
		if (inner(x,y-1) && AT(x,y-1)!=c)
			flood_fill(x,y-1,c);
		if (inner(x,y+1) && AT(x,y+1)!=c)
			flood_fill(x,y+1,c);
	}
}