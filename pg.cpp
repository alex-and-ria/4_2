#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <ctime>
#include <chrono>
#include<string>
#include <winsock2.h>
#include <windows.h>

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

unsigned short CalcChecksum (char *pBuffer, int nLen){
	//Checksum for ICMP is calculated in the same way as for
	//IP header
	//This code was taken from: http://www.netfor2.com/ipsum.htm
	unsigned short nWord;
	unsigned int nSum = 0;
	int i;
	//Make 16 bit words out of every two adjacent 8 bit words in the packet
	//and add them up
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

bool ValidateChecksum (char *pBuffer, int nLen){
	unsigned short nWord;
	unsigned int nSum = 0;
	int i;

	//Make 16 bit words out of every two adjacent 8 bit words in the packet
	//and add them up
	for (i = 0; i < nLen; i = i + 2)	{
		nWord =((pBuffer [i] << 8)& 0xFF00) + (pBuffer [i + 1] & 0xFF);
		nSum = nSum + (unsigned int)nWord;
	}
	//Take only 16 bits out of the 32 bit sum and add up the carries
	while (nSum >> 16)	{
		nSum = (nSum & 0xFFFF) + (nSum >> 16);
	}
	//To validate the checksum on the received message we don't complement the sum
	//of one's complement
	//One's complement the result
	//nSum = ~nSum;
	//The sum of one's complement should be 0xFFFF
	return ((unsigned short)nSum == 0xFFFF);
}

bool Initialize (){
	//Initialize WinSock
	WSADATA wsaData;
	if (WSAStartup (MAKEWORD (2, 2), &wsaData) == SOCKET_ERROR)	{
		cerr << endl << "An error occured in WSAStartup operation: " << "WSAGetLastError () = " << WSAGetLastError () << endl;
		return false;
	}
	/*SYSTEMTIME time;
	::GetSystemTime (&time);*/
	//Seed the random number generator with current millisecond value
	srand (time(NULL));
	return true;
}

bool UnInitialize (){
	//Cleanup
	if (WSACleanup () == SOCKET_ERROR)	{
		cerr << endl << "An error occured in WSACleanup operation: WSAGetLastError () = " << WSAGetLastError () << endl;
		return false;
	}
	return true;
}

bool ResolveIP (string& szRemoteHost, string& szIPAddress){
	hostent *pHostent = gethostbyname (szRemoteHost.c_str());
	if (pHostent == NULL){
		cerr << endl << "An error occured in gethostbyname operation: WSAGetLastError () = " << WSAGetLastError () << endl;
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
	if (argc < 2 || argc > 5){
		PrintUsage ();
		return 0;
	}
	if (Initialize () == false){
		return -1;
	}
	int nSequence = 0;
	int nTimeOut = 5000;	//Request time out for echo request (in milliseconds)
	int nMessageSize = 32;	//The message size that the ICMP echo request should carry with it
	int nCount = 4;	//Number of times the request has to be send
	string szRemoteIP, szRemoteHost(argv [1]); char* SendBuffer=NULL;
	/*cout<<"\nremoteHost="<<szRemoteHost;
	cout<<"\n\n";
	for(usi i=0;i<argc;i++){
		cout<<"\ni="<<i<<' '<<string(argv[i])<<'\n';
	}*/
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
	cout << "Pinging " << szRemoteHost << " [" << 
	(usi)in.S_un.S_un_b.s_b1<<'.'<<(usi)in.S_un.S_un_b.s_b2<<'.'<<(usi)in.S_un.S_un_b.s_b3<<'.'<<(usi)in.S_un.S_un_b.s_b4
	<< "] with " << nMessageSize << " bytes of data\n";
	
	//create socet;
	ICMPheader sendHdr; sendHdr.nId = htons (rand ());	//Set the transaction Id
	SOCKET sock= socket (AF_INET, SOCK_RAW, IPPROTO_ICMP);	//Create a raw socket which will use ICMP
	sockaddr_in dest;	//Dest address to send the ICMP request
	dest.sin_addr.S_un.S_addr = inet_addr (szRemoteIP.c_str());
	dest.sin_family = AF_INET;
	dest.sin_port = rand ();	//Pick a random port

	int nResult = 0;
	fd_set fdRead;
	SYSTEMTIME timeSend, timeRecv;//TODO;
	int nTotalRoundTripTime = 0, nMaxRoundTripTime = 0, nMinRoundTripTime = -1, nRoundTripTime = 0;
	int nPacketsSent = 0, nPacketsReceived = 0;
	timeval timeInterval = {0, 0};
	timeInterval.tv_usec = nTimeOut * 1000;
	//while (nPacketsSent < nCount){
		//create ICMP message;
		SendBuffer = new char [sizeof (ICMPheader) + nMessageSize];
		sendHdr.byType = 8;	//Eight for ICMP echo message
		sendHdr.byCode = 0;	//Zero for ICMP echo and reply messages
		sendHdr.nChecksum = 0;	//Checksum is calculated later on
		sendHdr.nSequence = htons (nSequence++);
		/*cout<<"\nb\n";
		for(usi i=0;i<sizeof (ICMPheader);i++){
			cout<<" SendBuffer["<<i<<"]="<<SendBuffer[i];
		}
		cout<<"\n\n sendHdr.byType="<<(usi)sendHdr.byType<<" sendHdr.byCode="<<(usi)sendHdr.byCode<<" sendHdr.nChecksum="
		<<(usi)sendHdr.nChecksum<<" sendHdr.nId="<<(usi)sendHdr.nId<<" sendHdr.nSequence="<<(usi)sendHdr.nSequence<<"\n\na\n";*/
		for(usi i=0;i<sizeof (ICMPheader);i++){
			SendBuffer[i]=((unsigned char*)&sendHdr)[i];
		}
		/*for(usi i=0;i<sizeof (ICMPheader);i++){
			cout<<" SendBuffer["<<i<<"]="<<(usi)SendBuffer[i];
		}*/
		for(unsigned int i=sizeof (ICMPheader);i<sizeof (ICMPheader) + nMessageSize;i++){
			SendBuffer[i]=(unsigned char)0;
		}
		//Calculate checksum over ICMP header and message data
		sendHdr.nChecksum = htons (CalcChecksum (SendBuffer, sizeof (ICMPheader) + nMessageSize));	
		for(usi i=0;i<sizeof (ICMPheader);i++){
			SendBuffer[i]=((unsigned char*)&sendHdr)[i];
		}
		//send;
		nResult = sendto (sock, SendBuffer, sizeof (ICMPheader) + nMessageSize, 0, (SOCKADDR *)&dest, sizeof (SOCKADDR_IN));
		::GetSystemTime (&timeSend);//TODO;
		nPacketsSent++;
		if (nResult == SOCKET_ERROR){
			cerr << endl << "An error occured in sendto operation: " << "WSAGetLastError () = " << WSAGetLastError () << endl;
			UnInitialize ();
			delete []SendBuffer;
			return -1;
		}
		FD_ZERO (&fdRead);//TODO;
		FD_SET (sock, &fdRead);
		if ((nResult = select (0, &fdRead, NULL, NULL, &timeInterval))== SOCKET_ERROR){
			cerr<<"\nAn error occured in select operation: "<<"WSAGetLastError="<<WSAGetLastError ()<<'\n';
			delete []pSendBuffer;
			return -1;
		}//TODO;
		//recieve;
		if (nResult>0 && FD_ISSET(sock, &fdRead)){
			char *RecvBuffer = new char [1500];//Allocate a large buffer to store the response
			if ((nResult = recvfrom (sock, RecvBuffer, 1500, 0, 0, 0))== SOCKET_ERROR){
				cerr<<"\nAn error occured in recvfrom operation: " << "WSAGetLastError () = "<<WSAGetLastError()<<'\n';
				UnInitialize ();
				delete []pSendBuffer;
				delete []pRecvBuffer;
				return -1;
			}
			::GetSystemTime (&timeRecv);//TODO;//Get the time at which response is received
			ICMPheader recvHdr;//We got a response so we construct the ICMP header and message out of it
			char *ICMPbuffer = NULL;
			ICMPbuffer = RecvBuffer + sizeof(IPheader);//The response includes the IP header as well,
			//so we move 20 bytes ahead to read the ICMP header
			int nICMPmsgLen = nResult - sizeof(IPheader); //ICMP message length is calculated by subtracting the IP header size
			//from the total bytes received
//////////////////////////////////////////
			//Construct the ICMP header
			memcpy_s (&recvHdr, sizeof (recvHdr), ICMPbuffer, sizeof (recvHdr));

			//Construct the IP header from the response
			IPheader ipHdr;
			memcpy_s (&ipHdr, sizeof (ipHdr), RecvBuffer, sizeof (ipHdr));

			recvHdr.nId = recvHdr.nId;
			recvHdr.nSequence = recvHdr.nSequence;
			recvHdr.nChecksum = ntohs (recvHdr.nChecksum);

			//Check if the response is an echo reply, transaction ID and sequence number are same
			//as for the request, and that the checksum is correct
			if (recvHdr.byType == 0 &&
				recvHdr.nId == sendHdr.nId &&
				recvHdr.nSequence == sendHdr.nSequence &&
				ValidateChecksum (pICMPbuffer, nICMPMsgLen)  && 
				memcmp (pSendBuffer + sizeof(ICMPheader), pRecvBuffer + sizeof (ICMPheader) + sizeof(IPheader), 
					nResult - sizeof (ICMPheader) - sizeof(IPheader)) == 0)
			{
				//All's OK
				int nSec = timeRecv.wSecond - timeSend.wSecond;
				if (nSec < 0)
				{
					nSec = nSec + 60;
				}

				int nMilliSec = abs (timeRecv.wMilliseconds - timeSend.wMilliseconds);

				int nRoundTripTime = 0;
				nRoundTripTime = abs (nSec * 1000 - nMilliSec);

				cout << "Reply from " << pszRemoteIP << ": bytes = " << nResult - sizeof (ICMPheader) - sizeof (IPheader) 
					<< ", time = " << nRoundTripTime << "ms, TTL = " << (int)ipHdr.byTtl << endl;

				nTotalRoundTripTime = nTotalRoundTripTime + nRoundTripTime;

				if (nMinRoundTripTime == -1)
				{
					nMinRoundTripTime = nRoundTripTime;
					nMaxRoundTripTime = nRoundTripTime;
				}
				else if (nRoundTripTime < nMinRoundTripTime)
				{
					nMinRoundTripTime = nRoundTripTime;
				}
				else if (nRoundTripTime > nMaxRoundTripTime)
				{
					nMaxRoundTripTime = nRoundTripTime;
				}

				++nPacketsReceived;
			}
			else
			{
				cout << "The echo reply is not correct!" << endl;
			}

			delete []pRecvBuffer;
		}
		else
		{
			cout << "Request timed out." << endl;
		}

		delete []pSendBuffer;
		
		
	//}
	
}



