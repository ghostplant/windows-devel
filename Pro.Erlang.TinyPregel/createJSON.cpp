#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	int n=(argc>=2)?atoi(argv[1]):10;
	float val=(argc>=3)?atof(argv[2]):1;

	putchar('[');
	for (int i=1;i<=n;i++) {
		printf("{%d, %f, [", i, val);
		bool first=true;
		for (int j=1;j<=n;j++)
			if (i!=j) {
				if (first)
					first=false;
				else
					putchar(',');
				printf("{%d, 1}", j);
			}
		printf("]}");
		if (i<n)
			putchar(','), putchar('\n');
		else
			putchar(']');
	}
	putchar('.');
	putchar('\n');
}
