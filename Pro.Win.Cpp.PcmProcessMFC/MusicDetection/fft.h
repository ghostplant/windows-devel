#include <math.h>

#define DOUBLE_PI 6.283185307179586476925286766559f
#define MAXN 4096

namespace fft {

	void fft2b(float xreal [], int p) {
		static float ximag[MAXN], wreal[MAXN], wimag[MAXN], treal, timag, ureal, uimag, arg;
		int n = (1 << p), m, k, i, j, t, a, b, index1, index2;

		for (i = 0; i < n; i++) {
			ximag[i] = 0, a = i, b = 0;
			for (j = 0; j < p; j++) {
				b = (b << 1) + (a & 1);
				a >>= 1;
			}
			if (b > i)
				arg = xreal[i], xreal[i] = xreal[b], xreal[b] = arg;
		}

		arg = -DOUBLE_PI / n;
		treal = cos(arg), timag = sin(arg);
		wreal [0] = 1.0, wimag [0] = 0.0;
		for (j = 1; j + j < n; j++) {
			wreal [j] = wreal [j - 1] * treal - wimag [j - 1] * timag;
			wimag [j] = wreal [j - 1] * timag + wimag [j - 1] * treal;
		}

		for (m = 2; m <= n; m <<= 1)
			for (k = 0; k < n; k += m)
				for (j = 0; j + j < m; j++) {
					index1 = k + j, index2 = index1 + (m >> 1);
					t = n * j / m;
					treal = wreal [t] * xreal [index2] - wimag [t] * ximag [index2];
					timag = wreal [t] * ximag [index2] + wimag [t] * xreal [index2];
					ureal = xreal [index1];
					uimag = ximag [index1];
					xreal [index1] = ureal + treal;
					ximag [index1] = uimag + timag;
					xreal [index2] = ureal - treal;
					ximag [index2] = uimag - timag;
				}
		for (i = 0; i < n; i++)
			xreal[i] = xreal[i] * xreal[i] + ximag[i] * ximag[i];
	}
}