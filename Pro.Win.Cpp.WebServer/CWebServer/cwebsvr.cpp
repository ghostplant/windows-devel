#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <windows.h>
#include "httpsocket.h"

#define MAX_STRING 1048576
typedef char string[MAX_STRING];

DWORD WINAPI ThreadProc(LPVOID lpArgs);

char hex(char c) {
	if (isdigit(c))
		return c-'0';
	else if (isupper(c))
		return c-'A'+10;
	else
		return c-'a'+10;
}

class Thread {
	HANDLE hThread;
	char webName[MAX_STRING],webPath[MAX_STRING],webIndex[MAX_STRING];
	int webPort;
	
public:
	virtual void run() {
		ServerSocket server(webPort);
		while (true) {
			Socket client=server.accept();
			char *buffer=new string,*msg=new string,*param=new string,*page=new string;
			int length=0;
			*buffer=0;
			int size=client.read(msg,MAX_STRING);
			msg[size]=0;
			int enter=0;
			for (int i=0;i<size;i++) {
				if (msg[i]=='\r')
					continue;
				if (msg[i]=='\n')
					enter++;
				else
					enter=0;
				if (enter==2) {
					++i;
					strcat(buffer,msg+i);
					break;
				}
			}
			char *url=strchr(msg,' ');
			if (url) {
				*(url++)=0;
				char *ver=strchr(url,' ');
				if (ver) {
					*(ver++)=0;
					if (*url=='/')
						++url;
					char *it;
					bool legal=true;
					for (it=url;*it;it++) {
						if (!isdigit(*it) && !isalpha(*it) && *it!='?') {
							legal=false;
							break;
						}
						if (*it=='?') {
							*(it++)=0;
							break;
						}
					}
					if (!legal)
						client.write("HTTP/1.1 200 OK\ncontent-type: text/html\n\n非法链接地址");
					else {
						char *dll=new char[MAX_PATH];
						if (*url)
							sprintf(dll,"%s\\%s.dll",webPath,url);
						else
							sprintf(dll,"%s\\%s.dll",webPath,webIndex);
						HINSTANCE hDLL=LoadLibrary(dll);
						if (hDLL) {
							client.write("HTTP/1.0 200 OK\ncontent-type: text/html\n\n");
							typedef void (*Servlet)(const char*,char*);
							Servlet servlet=(Servlet)GetProcAddress(hDLL, "servlet");
							size=0;
							if (*it && *buffer)
								strcat(buffer,"&");
							strcat(buffer,it);
							it=buffer;
							while (*it) {
								if (*it=='%') {
									param[size++]=(hex(it[1])<<4)+hex(it[2]);
									it+=2;
								} else if (*it=='&')
									param[size++]='\n';
								else
									param[size++]=*it;
								++it;
							}
							param[size]=0;
							puts(param);
							servlet(param,page);
							client.write(page);
						} else
							client.write("HTTP/1.1 200 OK\ncontent-type: text/html\n\n无法找到链接地址");
					}
				}
			}
			delete []param;
			delete []page;
			delete []msg;
			client.close();
		}
	}
	
	void start(const char *name,const char *path,const char *index,int port) {
		strcpy(webName,name);
		strcpy(webPath,path);
		strcpy(webIndex,index);
		webPort=port;
		hThread = CreateThread(0,0,ThreadProc,this,0,0);
	}
	
	void destroy() {
		TerminateThread(hThread,0);
	}
};

DWORD WINAPI ThreadProc(LPVOID lpArgs) {
	Thread* thread=(Thread*)lpArgs;
	thread->run();
	return 0;
}

string webName,webPath,webIndex;
int webPort;

int main() {
	static string catalog, buffer;
	GetPrivateProfileString("webconfig","count","0",buffer,MAX_PATH,".\\config.ini");
	int count=atoi(buffer);
	if (count)
		printf("Loading %d web instances...\n",count);
	else
		printf("No web instance exists.\n");
	for (int i=1;i<=count;i++) {
		sprintf(catalog,"instance%d",i);
		GetPrivateProfileString(catalog,"name","Untitled",buffer,MAX_PATH,".\\config.ini");
		strcpy(webName,buffer);
		GetPrivateProfileString(catalog,"port","80",buffer,MAX_PATH,".\\config.ini");
		webPort=atoi(buffer);
		GetPrivateProfileString(catalog,"path",".",buffer,MAX_PATH,".\\config.ini");
		strcpy(webPath,buffer);
		GetPrivateProfileString(catalog,"index","default",buffer,MAX_PATH,".\\config.ini");
		strcpy(webIndex,buffer);
		Thread *thread=new Thread();
		thread->start(webName,webPath,webIndex,webPort);
		printf("Instance %d '%s' started successfully!\n",i,webName);
	}
	while (count)
		Sleep(1000);
	return 0;
}