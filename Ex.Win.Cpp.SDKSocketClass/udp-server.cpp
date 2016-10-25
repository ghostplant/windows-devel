#include <stdlib.h>
#include <stdio.h>

#include "socket.h"

int main() {
	DatagramSocket datagram(80);
	sockaddr s;
	int len;
	static char msg[100];
	while (~(len = datagram.read(msg,sizeof(msg),&s))) {
		msg[len] = 0;
		if (!strcmp(msg,"exit"))
			break;
		strupr(msg);
		datagram.write(msg,strlen(msg),&s);
	}
} 
