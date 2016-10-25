#include <stdlib.h>
#include <stdio.h>

#include "socket.h"

int main() {
	ClientSocket client("127.0.0.1", 80);
	Socket socket = client.connect();
	static char line[100];
	while (gets(line)) {
		socket.puts(line);
		if (!strcmp(line,"exit"))
			break;
		socket.gets(line);
		puts(line);
	}
	client.close();
} 
