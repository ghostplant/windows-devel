#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void create_process(const char *path) {
	int ch=fork();
	if (!ch) {
		system(path);
		exit(0);
	}
}

int main() {
	static char line[1<<20];
	FILE *fp=fopen("./batch_linux","r");
	while (~fscanf(fp,"%s",line)) {
		create_process(line);
		sleep(2);
	}
	return 0;
}