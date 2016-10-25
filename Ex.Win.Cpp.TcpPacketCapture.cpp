#include <WINSOCK2.H>
#include<iostream>
#include<stdio.h>
#include <winsock2.h>
#pragma comment(lib,"WS2_32")
#pragma comment(lib,"Mpr")
using namespace std;

//IP���ݱ��ײ���ʽ
typedef struct _IPHEADER {
	unsigned char header_len:4;
	unsigned char version:4;   
	unsigned char tos;            // type of service
	unsigned short total_len;      // length of the packet
	unsigned short ident;          // unique identifier
	unsigned short flags;          
	unsigned char ttl;            
	unsigned char proto;          // protocol ( IP , TCP, UDP etc)
	unsigned short checksum;       
	unsigned int   sourceIP;
	unsigned int   destIP;
	unsigned char text[5000];
} IPHEADER;

//TCP�ײ���ʽ
struct TCPPacketHead {
	WORD SourPort;
	WORD DestPort;
	DWORD SeqNo;
	DWORD AckNo;
	BYTE HLen;
	BYTE Flag;
	WORD WndSize;
	WORD ChkSum;
	WORD UrgPtr;
};

//ICMP�ײ���ʽ
struct ICMPPacketHead {
	BYTE Type;
	BYTE Code;
	WORD ChkSum;
};

//UDP�ײ���ʽ
struct UDPPacketHead {
	WORD SourPort;
	WORD DestPort;
	WORD Len;
	WORD ChkSum;
};

//ץ�����򲿷�
int main()
{
	WORD wVersionRequested;
	WSADATA wsaData; 
	int  err;
	CHAR szHostName[128] = {0};
	HOSTENT* pHost = NULL;
	CHAR* pszIp = NULL;
	int   iNum = 0;
	SOCKADDR_IN sa;
	wVersionRequested = MAKEWORD( 2, 0 );
	err = WSAStartup( wVersionRequested, &wsaData );//��ʼ��winsock2
	SOCKET ssocket;
	ssocket = socket( AF_INET , SOCK_RAW , IPPROTO_IP ) ;
	int rcvtimeo = 5000 ; // 5 sec instead of 45 as default
	
	//�����׽ӿ�
	setsockopt( ssocket , SOL_SOCKET , SO_RCVTIMEO , (const char *)&rcvtimeo , sizeof(rcvtimeo) );
	
	sa.sin_family = AF_INET;
	sa.sin_port = htons(7000);
	pHost = gethostbyname(szHostName);//��ȡ����IP��ַ
	pszIp=inet_ntoa(*(in_addr*)pHost->h_addr_list[1]);
	sa.sin_addr.s_addr= inet_addr(pszIp);
	
	//�뱾���������а�
	bind(ssocket,(PSOCKADDR)&sa, sizeof(sa));
	
	DWORD dwBufferLen[10] ;
	DWORD dwBufferInLen= 1 ;
	DWORD dwBytesReturned = 0 ;
	
	//����һ���׽ӿڵ�ģʽ
	WSAIoctl( ssocket, _WSAIOW(IOC_VENDOR,1), &dwBufferInLen, sizeof(dwBufferInLen), &dwBufferLen, sizeof(dwBufferLen),&dwBytesReturned , NULL , NULL );
	char buf [1000] , *bufwork ;
	char *pSource , *pDest ;
	char   szSource [16] , szDest[16] , szErr [ 50 ];
	//���ü������ݰ�����Ŀ
	for (;;)
	{
		memset( buf , 0 , sizeof(buf) ) ;
		int len = recv( ssocket , buf , sizeof( buf ) , 0 ) ;
		bufwork = buf;
		IPHEADER * pIpHeader = (IPHEADER *)bufwork ;
		in_addr ina ;
		ina.S_un.S_addr = pIpHeader->sourceIP ;
		pSource = inet_ntoa( ina ) ;
		strcpy( szSource , pSource ) ;
		ina.S_un.S_addr = pIpHeader->destIP ;
		pDest = inet_ntoa( ina ) ;
		strcpy( szDest , pDest ) ;
		int length=pIpHeader->header_len;
		//char *itext1=pIpHeader->text;
		//char text[500];
		//strcpy(itext,itext1);
		//printf("datalen %d len %d IP %s ip %s",len,pIpHeader->header_len,szSource,szDest);
		cout<<"datalen:"<<len<<"; len:"<<length<<"; Դ��ַ:"<<szSource<<"; Ŀ�ĵ�ַ"<<szDest<<endl;
		//printf("\n");
		cout<<pIpHeader->text<<endl;
		//string s=pIpHeader->data;
		//char s1[100];
		//strcpy(text,s.c_str());
		//cout<<buf<<endl;
		int HdrLen, totallen;
		WORD sourport, destport;
		struct TCPPacketHead *pTCPHead;
		struct ICMPPacketHead *pICMPHead;
		struct UDPPacketHead *pUDPHead;
		HdrLen = pIpHeader->header_len&0xf;
		HdrLen *= 4;
		switch(pIpHeader->proto)
		{
		case IPPROTO_ICMP:
			{
				pICMPHead=(struct ICMPPacketHead *)(buf+HdrLen);
				
				//����ICMPЭ��
				break;
			}
		case IPPROTO_TCP:
			{
				pTCPHead=(struct TCPPacketHead *)(buf+HdrLen);
				sourport = ntohs(pTCPHead->SourPort);
				destport = ntohs(pTCPHead->DestPort);
				printf("tcp sourport %d destport %d",sourport,destport);
				//����tcpЭ��
				break;
			}
		case IPPROTO_UDP:
			{
				pUDPHead=(struct UDPPacketHead *)(buf+HdrLen);
				//����UDPЭ��
				break;
			}
		}
		printf("\n");
		printf("\n");
	}
	
	return 0;
}