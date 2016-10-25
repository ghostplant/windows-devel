#include <stdlib.h>
#include <stdio.h>

#include "socket.h"

int main() {
	ServerSocket server(80);
	Socket socket = server.accept();
	static char line[100];
	while (~socket.gets(line)) {
		if (!strcmp(line,"exit"))
			break;
		socket.puts(line);
	}
	server.close();
} 
