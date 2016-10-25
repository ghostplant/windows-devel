#include <winsock.h>
#pragma comment(lib,"WSOCK32.LIB")

class Socket {
	SOCKET s;
	sockaddr_in locals;

public:

	Socket(SOCKET sock, const sockaddr_in &remotes) {
		s = sock;
		locals = remotes;
	}

	int read(char *buf,int sz) {
		return recv(s, buf, sz, 0);
	}

	void write(const char *buf,int sz = -1) {
		if (sz == -1)
			sz = strlen(buf);
		send(s, buf, sz, 0);
	}

	void close() {
		closesocket(s);
	}
};

class ServerSocket {
	SOCKET s;
	WSADATA WD;
	sockaddr_in locals;

public:
	ServerSocket(int Port,int Listen = 100) {
		locals.sin_family = AF_INET;
		locals.sin_port = htons(Port);
		locals.sin_addr.s_addr = htonl(INADDR_ANY);

		WSAStartup(0x0101, (LPWSADATA)&WD);
		s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		bind(s,(sockaddr*)&locals, sizeof(sockaddr));
		listen(s,Listen);
	}

	Socket accept() {
		sockaddr_in remotes;
		int size = sizeof(sockaddr);
		SOCKET sock = ::accept(s,(sockaddr*)&remotes, &size);
		return Socket(sock, remotes);
	}

	void close() {
		closesocket(s);
		WSACleanup();
	}
};
