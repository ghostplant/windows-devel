#include <stdlib.h>
#include <stdio.h>

#include "socket.h"

int main() {
	DatagramSocket datagram;
	sockaddr s = DatagramSocket::remoteTarget("127.0.0.1",80);
	int len;
	static char msg[100];
	while (gets(msg)) {
		if (!strcmp(msg,"exit"))
			break;
		datagram.write(msg,sizeof(msg),&s);
		int len=datagram.read(msg,sizeof(msg),&s);
		msg[len] = 0;
		puts(msg);
	}
} 
