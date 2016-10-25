#include <winsock.h>
#pragma comment(lib,"WSOCK32.LIB")

class Socket {
	SOCKET s;
	sockaddr_in locals;

public:
	Socket() {
	}

	Socket(SOCKET sock, const sockaddr_in &remotes) {
		s=sock;
		memcpy(&locals,&remotes,sizeof(sockaddr_in));
	}

	int gets(char *buf,int len) {
		return recv(s, buf, len, 0);
	}

	void puts(char *buf,int len) {
		send(s, buf, len, 0);
	}

	void close() {
		closesocket(s);
	}
};

class ClientSocket {
	SOCKET s;
	WSADATA WD;
	sockaddr_in remotes;

public:
	ClientSocket(const char *ipAddr,int Port) {
		remotes.sin_family=AF_INET;
		remotes.sin_port=htons(Port);
		remotes.sin_addr.s_addr=inet_addr(ipAddr);

		WSAStartup(0x0101, (LPWSADATA)&WD);
		s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}

	Socket connect() {
		::connect(s,(sockaddr*)&remotes,sizeof(sockaddr));
		return Socket(s, remotes);
	}

	void close() {
		closesocket(s);
		WSACleanup();
	}
};
