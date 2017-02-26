
#include <iostream>
#include <ctime>
#include <chrono>
#include<string>
#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#elif __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#endif

using namespace std;
typedef unsigned short int usi;

struct ICMPheader{
	unsigned char	byType;
	unsigned char	byCode;
	unsigned short	nChecksum;
	unsigned short	nId;
	unsigned short	nSequence;
};

struct IPheader{
    unsigned char	byVerLen;
    unsigned char	byTos;
    unsigned short	nTotalLength;
    unsigned short	nId;
    unsigned short	nOffset;
    unsigned char	byTtl;
    unsigned char	byProtocol;
    unsigned short	nChecksum;
    unsigned int	nSrcAddr;
    unsigned int	nDestAddr;
};

unsigned short CalcChecksum (unsigned char *pBuffer, int nLen){
	//Checksum for ICMP is calculated in the same way as for IP header
	//This code was taken from: http://www.netfor2.com/ipsum.htm
	unsigned short nWord;
	unsigned int nSum = 0;
	int i;
	//Make 16 bit words out of every two adjacent 8 bit words in the packet and add them up
	for (i = 0; i < nLen; i = i + 2)	{
		nWord =((pBuffer [i] << 8)& 0xFF00) + (pBuffer [i + 1] & 0xFF);
		nSum = nSum + (unsigned int)nWord;
	}
	//Take only 16 bits out of the 32 bit sum and add up the carries
	while (nSum >> 16)	{
		nSum = (nSum & 0xFFFF) + (nSum >> 16);
	}
	//One's complement the result
	nSum = ~nSum;
	return ((unsigned short) nSum);
}

bool ValidateChecksum (unsigned char *pBuffer, int nLen){
	unsigned short nWord;
	unsigned int nSum = 0;
	int i;
	//Make 16 bit words out of every two adjacent 8 bit words in the packet and add them up
	for (i = 0; i < nLen; i = i + 2)	{
		nWord =((pBuffer [i] << 8)& 0xFF00) + (pBuffer [i + 1] & 0xFF);
		nSum = nSum + (unsigned int)nWord;
	}
	//Take only 16 bits out of the 32 bit sum and add up the carries
	while (nSum >> 16)	{
		nSum = (nSum & 0xFFFF) + (nSum >> 16);
	}
	//To validate the checksum on the received message we don't complement the sum of one's complement
	//One's complement the result nSum = ~nSum;
	//The sum of one's complement should be 0xFFFF
	return ((unsigned short)nSum == 0xFFFF);
}
#ifdef _WIN32
bool Initialize (){
	WSADATA wsaData;//Initialize WinSock
	if (WSAStartup (MAKEWORD (2, 2), &wsaData) == SOCKET_ERROR)	{
		cerr << endl << "An error occured in WSAStartup operation: " << "WSAGetLastError () = " << WSAGetLastError () << endl;
		return false;
	}
	srand(time(NULL)); //Seed the random number generator
	return true;
}

bool UnInitialize (){	
	if (WSACleanup () == SOCKET_ERROR)	{//Cleanup
		cerr << endl << "An error occured in WSACleanup operation: WSAGetLastError () = " << WSAGetLastError () << endl;
		return false;
	}
	return true;
}
#endif

bool ResolveIP (string& szRemoteHost, string& szIPAddress){
	hostent *pHostent = gethostbyname (szRemoteHost.c_str());
	if (pHostent == NULL){
		cerr << endl << "An error occured in gethostbyname operation: WSAGetLastError () = ";
		#ifdef _WIN32
		 cout<< WSAGetLastError () << endl;
		#endif
		return false;
	}
	szIPAddress=string(pHostent->h_addr_list[0]);
	return true;
}

void PrintUsage (){
	cout << "Usage: ping r n b t" << endl << endl;
	cout << "  r - Remote host" << endl;
	cout << "  n - Number of echo requests to send" << endl;
	cout << "  b - Bytes to send" << endl;
	cout << "  t - Timeout after these many milliseconds" << endl << endl;
	cout << "\rping microsoft.com 4 32 4000" << endl << endl;
}

int main(int argc, char** argv){
	if (argc > 5){
		PrintUsage ();
		return 0;
	}
	#ifdef _WIN32
	if (Initialize () == false){
		return -1;
	}
	#endif
	int nSequence = 0;
	int nTimeOut = 5000;	//Request time out for echo request (in milliseconds)
	int nMessageSize = 32;	//The message size that the ICMP echo request should carry with it
	int nCount = 4;	//Number of times the request has to be send
	string szRemoteIP, szRemoteHost;
	if(argc>=2) szRemoteHost=string(argv [1]); else szRemoteHost=string("www.google.com");
	char* SendBuffer=NULL;
	//get param;
	switch(argc){
		case 5:
			nTimeOut = atoi (argv [4]);
			cout<<"\nargc="<<argc<<" nTimeOut="<<nTimeOut;
		case 4:
			nMessageSize = atoi (argv [3]);
			cout<<"\nargc="<<argc<<" nMessageSize="<<nMessageSize;
		case 3:
			nCount = atoi (argv [2]);
			cout<<"\nargc="<<argc<<" nCount="<<nCount;
	}
	//cout<<"\nnTimeOut="<<nTimeOut<<" nMessageSize="<<nMessageSize<<" nCount="<<nCount;
	//get ip;
	if (ResolveIP (szRemoteHost, szRemoteIP) == false){
		cerr << endl << "Unable to resolve hostname" << endl;
		return -1;
	}
	in_addr in; in.s_addr = *(u_long *) szRemoteIP.c_str();
	cout << "\nPinging " << szRemoteHost << " [";
	#ifdef _WIN32
	cout<<(usi)in.S_un.S_un_b.s_b1<<'.'<<(usi)in.S_un.S_un_b.s_b2<<'.'<<(usi)in.S_un.S_un_b.s_b3<<'.'<<(usi)in.S_un.S_un_b.s_b4;
	#elif __linux__
	cout<<inet_ntoa(in);
	#endif
	cout<< "] with " << nMessageSize << " bytes of data\n";
	//create socet;
	ICMPheader sendHdr; sendHdr.nId = htons (rand ());	//Set the transaction Id
	#ifdef _WIN32
	SOCKET sock;
	#elif __linux__
	int sock;
	#endif
	sock= socket (AF_INET, SOCK_RAW, IPPROTO_ICMP);	//Create a raw socket which will use ICMP
	sockaddr_in dest; memset(&dest, 0, sizeof(dest));//Dest address to send the ICMP request
	memcpy(&(dest.sin_addr), szRemoteIP.c_str(),sizeof(szRemoteIP.c_str()));//dest.sin_addr.S_un.S_addr = inet_addr (szRemoteIP.c_str());
	dest.sin_family = AF_INET;

	int nResult = 0; chrono::time_point<std::chrono::system_clock> tsend, trecv;
	//fd_set fdRead; SYSTEMTIME timeSend, timeRecv;//TODO;
	int nTotalRoundTripTime = 0, nMaxRoundTripTime = 0, nMinRoundTripTime = -1;
	int nPacketsSent = 0, nPacketsReceived = 0;
	//timeval timeInterval = {0, 0}; timeInterval.tv_usec = nTimeOut * 1000;
	while (nPacketsSent < nCount){
		//create ICMP message;
		SendBuffer = new char [sizeof (ICMPheader) + nMessageSize];
		sendHdr.byType = 8;	//Eight for ICMP echo message
		sendHdr.byCode = 0;	//Zero for ICMP echo and reply messages
		sendHdr.nChecksum = 0;	//Checksum is calculated later on
		sendHdr.nSequence = htons (nSequence++);
		memcpy(SendBuffer,&sendHdr,sizeof(ICMPheader));
		memset(SendBuffer+sizeof (ICMPheader),0,nMessageSize);
		//Calculate checksum over ICMP header and message data
		sendHdr.nChecksum = htons (CalcChecksum ((unsigned char*)SendBuffer, sizeof (ICMPheader) + nMessageSize));
		memcpy(SendBuffer,&sendHdr,sizeof(ICMPheader));
		//send;
		tsend=chrono::system_clock::now();
		nResult = sendto (sock, SendBuffer, sizeof (ICMPheader) + nMessageSize, 0, (struct sockaddr*)&dest, sizeof (sockaddr_in));		
		nPacketsSent++;
		#ifdef _WIN32
			auto sckerr=SOCKET_ERROR;
		#elif __linux__
			auto sckerr=-1;
		#endif
		if (nResult == sckerr){
			cerr << endl << "An error occured in sendto operation: " << "WSAGetLastError () = ";
		#ifdef _WIN32
		 cout<< WSAGetLastError () << endl; UnInitialize ();
		#endif
			delete []SendBuffer;
			return -1;
		}		
		//recieve;
		chrono::duration<double,milli> ds=chrono::system_clock::now()-tsend;
		if (nResult>0 && ds.count()<=((double)nTimeOut)){
			unsigned char *RecvBuffer = new unsigned char [1500];//Allocate a large buffer to store the response
			if ((nResult = recvfrom (sock, (char*)RecvBuffer, 1500, 0, 0, 0))== sckerr){
				cerr<<"\nAn error occured in recvfrom operation: " << "WSAGetLastError () = ";
		#ifdef _WIN32
		 cout<< WSAGetLastError () << endl; UnInitialize ();
		#endif
				delete []SendBuffer;
				delete []RecvBuffer;
				return -1;
			}
			trecv=chrono::system_clock::now();//::GetSystemTime (&timeRecv);//TODO;//Get the time at which response is received
			ICMPheader recvHdr;//We got a response so we construct the ICMP header and message out of it
			IPheader ipHdr;
			unsigned char *ICMPbuffer= RecvBuffer + sizeof(IPheader);//The response includes the IP header as well,
			//so we move 20 bytes ahead to read the ICMP header
			int nICMPmsgLen = nResult - sizeof(IPheader); //ICMP message length is calculated by subtracting the IP header size
			//from the total bytes received
			memcpy(&recvHdr,ICMPbuffer,sizeof(ICMPheader));//Construct the ICMP header
			memcpy(&ipHdr,RecvBuffer,sizeof(IPheader));//Construct the IP header from the response
			recvHdr.nChecksum = ntohs (recvHdr.nChecksum);
			//check			
			if (recvHdr.byType == 0 &&//Check if the response is an echo reply, transaction ID and sequence number are same
			recvHdr.nId == sendHdr.nId && recvHdr.nSequence == sendHdr.nSequence &&
			ValidateChecksum (ICMPbuffer, nICMPmsgLen)  && //as for the request, and that the checksum is correct
			memcmp(SendBuffer+sizeof(ICMPheader),RecvBuffer+sizeof(ICMPheader)+sizeof(IPheader),
			nResult-sizeof(ICMPheader)-sizeof(IPheader))==0){//All's OK
				/*int nSec = timeRecv.wSecond - timeSend.wSecond;//TODO;
				if (nSec < 0){
					nSec = nSec + 60;
				}*/
				//int nMilliSec = abs (timeRecv.wMilliseconds - timeSend.wMilliseconds);
				chrono::duration<double,milli> rtt=trecv-tsend;
				//int nRoundTripTime=abs (nSec * 1000 - nMilliSec);
				cout << "Reply " << nPacketsReceived << ": bytes = " << nResult - sizeof (ICMPheader) - sizeof (IPheader) 
					<< ", time = " << rtt.count() << "ms, TTL = " << (int)ipHdr.byTtl << endl;

				nTotalRoundTripTime = nTotalRoundTripTime + rtt.count();
				if (nMinRoundTripTime == -1){
					nMinRoundTripTime = rtt.count();;
					nMaxRoundTripTime = rtt.count();;
				}
				else if (rtt.count()< nMinRoundTripTime){
					nMinRoundTripTime = rtt.count();;
				}
				else if (rtt.count()> nMaxRoundTripTime){
					nMaxRoundTripTime = rtt.count();;
				}
				++nPacketsReceived;
			}
			else{
				cout << "The echo reply is not correct!" << endl;
			}
			delete []RecvBuffer;
		}
		else{
			cout << "Request timed out." << endl;
		}
		delete []SendBuffer;		
	}
}
