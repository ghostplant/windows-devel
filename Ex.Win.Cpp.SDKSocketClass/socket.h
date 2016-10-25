#include <winsock.h>
#pragma comment(lib,"WSOCK32.LIB")

class Socket {
	SOCKET s;
	sockaddr_in locals;

public:

	Socket(SOCKET sock, const sockaddr_in &remotes) {
		s=sock;
		memcpy(&locals,&remotes,sizeof(sockaddr_in));
	}

	int gets(char *buf,char brk = '\n') {
		int l = 0, c = 0;
		while (~recv(s, (char*)&c, 1, 0)) {
			if (c == brk) {
				buf[l] = 0;
				return l;
			} else
				buf[l++] = c;
		}
		return -1;
	}

	void puts(char *buf) {
		send(s, buf, strlen(buf), 0);
		char ln = '\n';
		send(s, &ln, 1, 0);
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

class DatagramSocket {
	SOCKET s;
	WSADATA WD;
	sockaddr_in locals;

public:
	DatagramSocket(int Port = -1) {
		locals.sin_family = AF_INET;
		locals.sin_port = htons(Port);
		locals.sin_addr.s_addr = htonl(INADDR_ANY);

		WSAStartup(0x0101, (LPWSADATA)&WD);
		s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (~Port)
			bind(s,(sockaddr*)&locals, sizeof(sockaddr));
	}

	static sockaddr remoteTarget(const char *ipAddr,int Port) {
		sockaddr_in remotes;
		remotes.sin_family = AF_INET;
		remotes.sin_port = htons(Port);
		remotes.sin_addr.s_addr = inet_addr(ipAddr);
		return *((sockaddr*)&remotes);
	}

	void write(const char *Msg,int size,sockaddr *remotes) {
		sendto(s,Msg,size,0,remotes,sizeof(sockaddr));
	}

	int read(char *Msg,int size, sockaddr *remotes) {
		int sz=sizeof(sockaddr);
		return recvfrom(s,Msg,size,0,remotes,&sz);
	}
};
