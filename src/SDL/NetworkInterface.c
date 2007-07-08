#include <string.h>
#include <stdio.h>
#include "SDL.h"
#include "SDL_thread.h"
#ifdef HW_ENABLE_NETWORK
#include "SDL_net.h"
#endif
#include "NetworkInterface.h"

#ifdef HW_ENABLE_NETWORK


SDL_Thread *listenBroadcast, *listenTCP;
static UDPsocket broadcastSendSock, broadcastRecvSock;
static Client* TCPClientsConnected;
static TCPsocket clientSock;
static int endNetwork, clientActive, numTCPClientConnected;
static SDL_sem *semList;

void initNetwork()
{
	Uint32 sdl_flags;
	IPaddress broadcastIp;
	broadcastIp.host = INADDR_BROADCAST;
	broadcastIp.port = UDPPORT;
	endNetwork = 1;
	clientActive = 0;
	numTCPClientConnected = 0;

	semList = SDL_CreateSemaphore(1);

	/* Make sure SDL is initialized. */
	sdl_flags = SDL_WasInit(SDL_INIT_EVERYTHING);
	if (!sdl_flags)
	{
		if(SDL_Init(0) == -1)
		{
			exit(0);
		}
	}

	if(SDLNet_Init() == -1)
	{
		exit(1);
	}

	// Initialisation of Broadcast Sockets

	// Open Udp Port to send Broadcast Packet
	if(!(broadcastRecvSock = SDLNet_UDP_Open(UDPPORT)))
	{
		exit(3);
	}

	// open udp client socket
	if(!(broadcastSendSock=SDLNet_UDP_Open(0)))
	{
		exit(5);
	}

	// Create Thread to listen to Broadcast Packet on UDP
	listenBroadcast = SDL_CreateThread(broadcastStartThread,broadcastSendSock);
	if(!listenBroadcast)
	{
		exit(4);
	}

        if(SDLNet_ResolveHost(&broadcastIp,"255.255.255.255",UDPPORT)==-1)
	{
		printf("can't resolv IP given\n");
		exit(2);
	}

	// bind server address to channel 0
	if(SDLNet_UDP_Bind(broadcastSendSock, 0, &broadcastIp)==-1)
	{
	 	exit(6);
	}


	// Initialisation of TCP Server Thread

	listenTCP = SDL_CreateThread(TCPServerStartThread,NULL);
	if(!listenBroadcast)
	{
		exit(4);
	}

}

void sendBroadcastPacket(const void* packet, int len)
{
	UDPpacket *out = SDLNet_AllocPacket(2048);
	memcpy(out->data,packet,len);
	out->len=len;
	if(!SDLNet_UDP_Send(broadcastSendSock, 0, out))
	{
		exit(10);
	}
	SDLNet_FreePacket(out);
}

void shutdownNetwork()
{
	endNetwork = 0;
	SDLNet_UDP_Close(broadcastSendSock);
	broadcastSendSock = NULL;
	SDL_WaitThread(listenBroadcast,NULL);
	SDLNet_Quit();
}

int broadcastStartThread(void *data)
{
	Uint32 ipaddr;
	UDPpacket *packet = SDLNet_AllocPacket(2048);
	IPaddress newIp;
	int number;
	IpList listIps = NULL;

//	unsigned int begin = SDL_GetTicks();
	while(endNetwork)
	{
		if(SDLNet_UDP_Recv(broadcastRecvSock,packet)>0)
		{
//			printf("Packet received, length: %d id: %s port: %d\n", packet->len, packet->data,packet->address.port);
			ipaddr=SDL_SwapBE32(packet->address.host);
//			printf("IP Address : %d.%d.%d.%d\n",
//						ipaddr>>24,
//						(ipaddr>>16)&0xff,
//						(ipaddr>>8)&0xff,
//						ipaddr&0xff);

			SDLNet_ResolveHost(&newIp,"255.255.255.255",UDPPORT);
			newIp.host=packet->address.host;
			if(checkList(newIp, listIps) == -1)
			{
				printf("newIp : %d\n",newIp.host);
				listIps = addList(newIp, listIps);
				if((number=SDLNet_UDP_Bind(data,0,&newIp))==-1)
				{
					printf("error binding\n");
				}
				else
					printf("binding last ip received, channel %d\n",number);
			}
//			printf("packet recu taille : %d\n",packet->len);
			titanReceivedLanBroadcastCB(packet->data, packet->len);
		}
		else
			SDL_Delay(100);
	}
	
	SDLNet_UDP_Close(broadcastRecvSock);
	broadcastRecvSock = NULL;
	
	return 0;
}

int checkList(IPaddress Ip, IpList list)
{
	IpList tmp = list;
	while(tmp != NULL)
	{
		if(tmp->IP.host == Ip.host)
			return 0;
		else
			tmp = tmp->nextIP;
	}
//	printf("not found in the list\n");

	return -1;
}

IpList addList(IPaddress newIp, IpList list)
{
	IpElem *new = (IpElem *)malloc(sizeof(IpElem));
	new->IP = newIp;
	new->nextIP = list;
	list = new;
	return list;
}


// TODO : Sometimes bad packet is received, which will make the connection hang

Uint32 getMyAddress()
{
        UDPsocket recvSock;
        UDPpacket *packet = SDLNet_AllocPacket(512);
	Uint32 ipAdd;

        printf("thread created\n");

        if(!(recvSock=SDLNet_UDP_Open(45268)))
        {
                printf("socket not open\n");
                exit(5);
        }

        SDL_Thread *pingRecv = SDL_CreateThread(pingSendThread,NULL);

        while(SDLNet_UDP_Recv(recvSock,packet)<=0);


        printf("Packet received, length: %d id: %s port: %d\n", packet->len, packet->data,packet->address.port);
        Uint32 ipaddr=SDL_SwapBE32(packet->address.host);
        printf("IP Address : %d.%d.%d.%d\n",
                                        ipaddr>>24,
                                        (ipaddr>>16)&0xff,
                                        (ipaddr>>8)&0xff,
                                        ipaddr&0xff);
	printf("IP Address : %d\n",packet->address.host);
	ipAdd = packet->address.host;
        SDLNet_FreePacket(packet);

        SDL_WaitThread(pingRecv,NULL);
        SDLNet_UDP_Close(recvSock);
	printf("ipAdd : %d\n",ipAdd);
	return ipAdd;
}

int pingSendThread(void *data)
{
        UDPsocket sendSock;
        IPaddress pingIp;
        pingIp.port = 45268;


        if(SDLNet_ResolveHost(&pingIp,NULL,45268)==-1)
        {
                exit(2);
        }
        pingIp.host = INADDR_BROADCAST;

        UDPpacket *out = SDLNet_AllocPacket(512);

        if(!(sendSock=SDLNet_UDP_Open(0)))
        {
                printf("socket not open\n");
                exit(5);
        }

        if(!SDLNet_UDP_Bind(sendSock,1,&pingIp ))
        {
                exit(10);
        }


        SDL_Delay(100);

        out->len = 10;
        char* message = "ping";
        memcpy(out->data,message,strlen(message)+1);

        if(!SDLNet_UDP_Send(sendSock,1,out))
        {
                printf("no destination send\n");
        }
        SDLNet_FreePacket(out);
        SDLNet_UDP_Close(sendSock);
        return 0;
}



Uint32 connectToServer(Uint32 serverIP)
{
	IPaddress ipToConnect;
	Uint32 ipViewed;
	int numrdy, result;
	
	if(SDLNet_ResolveHost(&ipToConnect,NULL,TCPPORT)==-1)
	{
		exit(2);
	}

	printf("addresse recu en paramètre %d\n",serverIP);
	ipToConnect.host = serverIP;

	if(!(clientSock = SDLNet_TCP_Open(&ipToConnect)))
	{
		printf("can't create Server TCP socket \n");
		exit(3);
	}

	SDLNet_SocketSet set;
	set = SDLNet_AllocSocketSet(1);
	SDLNet_TCP_AddSocket(set, clientSock);
	numrdy=SDLNet_CheckSockets(set, (Uint32)-1);

	if(SDLNet_SocketReady(clientSock))
	{
		result=SDLNet_TCP_Recv(clientSock,&ipViewed,sizeof(Uint32));
		if(result<sizeof(Uint32))
		{
			printf("SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
			return NULL;
		}
	}
	else
		printf("Error during connection");


	// Switching to Client Mode.
	clientActive = 1;
	
	
	printf("Manage to connect");
	
	//addSockToList(sock);
	return ipViewed;	
}


int TCPServerStartThread(void *data)
{
	TCPsocket serverTCPSock, sock;
	SDLNet_SocketSet setSock, clientSet;
	IPaddress ip;
	IPaddress* fromIp;
	int clientInSet, i;
	clientInSet = 0;
	unsigned short lenPacket;
	unsigned char typMsg;
	Uint8* packet = NULL;


	// Resolve the argument into an IPaddress type
	if(SDLNet_ResolveHost(&ip,NULL,TCPPORT)==-1)
	{
		exit(2);
	}

	if(!(serverTCPSock = SDLNet_TCP_Open(&ip)))
	{
		printf("can't create Server TCP socket \n");
		exit(3);
	}	


	clientSet = SDLNet_AllocSocketSet(1);
	if(!clientSet)
		exit(1);

	setSock = SDLNet_AllocSocketSet(100);
	if(!setSock)
		exit(1);

	SDLNet_TCP_AddSocket(setSock, serverTCPSock);

						printf("size of packet received %d\n",lenPacket);
						printf("message type %d\n",typMsg);
		

	while(endNetwork)
	{
		int numready;
	
		if(clientActive == 0)
		{
			// In case no client connected, we might switch to Client type.
			if(numTCPClientConnected == 0)
			{
				numready=SDLNet_CheckSockets(setSock, (Uint32)200);
//				printf("Testing for 100ms\n");
			}
			else
				numready=SDLNet_CheckSockets(setSock, (Uint32)-1);

			if(numready==-1)
			{
				printf("SDLNet_CheckSockets: %s\n",SDLNet_GetError());
				break;
			}

			if(!numready)
				continue;
			if(SDLNet_SocketReady(serverTCPSock))
			{
				numready--;
				//printf("Connection...\n");
				sock=SDLNet_TCP_Accept(serverTCPSock);
				if(sock)
				{
					printf("New connection\n");
					SDLNet_TCP_AddSocket(setSock, sock);
					addSockToList(sock);
					fromIp = SDLNet_TCP_GetPeerAddress(sock);
					int res;
					res = SDLNet_TCP_Send(sock,&(fromIp->host),sizeof(Uint32));
					if(res<sizeof(Uint32)) {
						printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
						printf("Error sending back Ip");
					}
					
				}
				else
					printf("No new connection\n");
			}
			for(i=0; numready && i<numTCPClientConnected; i++)
			{
				if(SDLNet_SocketReady(TCPClientsConnected[i].sock))
				{
//					printf("New packet incoming from client %d\n",i);

					if(getPacket(TCPClientsConnected[i].sock, &typMsg, &packet, &lenPacket))
					{
						numready--;
//						printf("size of packet received %d\n",lenPacket);
//						printf("message type %d\n",typMsg);
						fromIp = SDLNet_TCP_GetPeerAddress(TCPClientsConnected[i].sock);
						HandleTCPMessage(fromIp->host, typMsg, packet, lenPacket);
					}
					else
						removeSockFromList(i);
				}
			}

		}
		else
		{
//			printf("Client server\n");
			if(clientInSet == 0)
			{
				printf("Adding socket to set\n");
				SDLNet_TCP_AddSocket(clientSet, clientSock);
				clientInSet = 1;
			}

			numready=SDLNet_CheckSockets(clientSet, (Uint32)-1);
			
			// Code as a client
			if(SDLNet_SocketReady(clientSock))
			{
				numready--;
//				printf("New packet incoming from server\n");

				if(getPacket(clientSock, &typMsg, &packet, &lenPacket))
				{
//					printf("size of packet received %d\n",lenPacket);
//					printf("message type %d\n",typMsg);
					fromIp = SDLNet_TCP_GetPeerAddress(clientSock);
					HandleTCPMessage(fromIp->host, typMsg, packet, lenPacket);
				}
				else
				{
					SDLNet_TCP_Close(clientSock);
					clientInSet = 0;
					clientActive = 0;
				}

			}

		}

	}
	return 0;
}


Client * addSockToList(TCPsocket sock)
{
	IPaddress *remoteIp;
	remoteIp = SDLNet_TCP_GetPeerAddress(sock);

	if(remoteIp != NULL)	
	{
		SDL_SemWait(semList);
		TCPClientsConnected = (Client*) realloc (TCPClientsConnected, (numTCPClientConnected+1)*sizeof(Client));
		TCPClientsConnected[numTCPClientConnected].sock = sock;
		TCPClientsConnected[numTCPClientConnected].IP = *remoteIp;
		numTCPClientConnected++;
		SDL_SemPost(semList);
		return (&TCPClientsConnected[numTCPClientConnected-1]);
	}
	else
		return NULL;
}

void removeSockFromList(int num)
{
	SDL_SemWait(semList);
	SDLNet_TCP_Close(TCPClientsConnected[num].sock);
	numTCPClientConnected--;

	if(num < numTCPClientConnected)
		memmove(&TCPClientsConnected[num], &TCPClientsConnected[num+1], (numTCPClientConnected-num)*sizeof(Client));
	TCPClientsConnected = (Client*) realloc (TCPClientsConnected, numTCPClientConnected*sizeof(Client));
	SDL_SemPost(semList);

}

TCPsocket findSockInList(Uint32 addressSock)
{
	TCPsocket sock;
	
	int i;
	for (i=0; i < numTCPClientConnected; i++)
		if (addressSock == TCPClientsConnected[i].IP.host)
			return TCPClientsConnected[i].sock;
	return NULL;
}


// Implementation of putPacket and getPacket which are Homeworld specific
// In homeworld with each message, a message type is send
// We first send the message type, then the message len, and finally the message data


void putPacket(Uint32 address, unsigned char msgType, const void* data, unsigned short dataLen)
{
	int result;
	TCPsocket sock;

	if(clientActive == 0)
	{
//		printf("envoie en tant que serveur\n",dataLen);
		SDL_SemWait(semList);
		sock = findSockInList(address);
		result = SDLNet_TCP_Send(sock,&msgType,sizeof(unsigned char));
		if(result<sizeof(unsigned char)) {
			printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());

		}

		result = SDLNet_TCP_Send(sock,&dataLen,sizeof(unsigned short));

		if(result<sizeof(unsigned short)) {
			printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());

		}
		result = SDLNet_TCP_Send(sock,data,dataLen);
		if(result<dataLen) {
			printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());

		}
		SDL_SemPost(semList);
	}
	else
	{
//		printf("envoie en tant que client\n",dataLen);
//		printf("message type %d\n",msgType);
		sock = clientSock;
		result = SDLNet_TCP_Send(sock,&msgType,sizeof(unsigned char));
		if(result<sizeof(unsigned char)) {
			printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());

		}

		result = SDLNet_TCP_Send(sock,&dataLen,sizeof(unsigned short));

		if(result<sizeof(unsigned short)) {
			printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());

		}
		result = SDLNet_TCP_Send(sock,data,dataLen);
		if(result<dataLen) {
			printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());

		}
	}
//	printf("packet send, size of the packet :  %d\n",dataLen);
}


unsigned char getPacket(TCPsocket sock, unsigned char* msgType, Uint8** packetData, unsigned short* packetLen)
{
	int result;
	
	if(*packetData)
		free(*packetData);
	*packetData = NULL;

	result=SDLNet_TCP_Recv(sock,msgType,sizeof(unsigned char));
	if(result<sizeof(unsigned char))
	{
//		printf("SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
		printf("Other side must have quit");
		return NULL;
	}

//	printf("type of message received %d\n",*msgType);

	result=SDLNet_TCP_Recv(sock,packetLen,sizeof(unsigned short));
	if(result<sizeof(unsigned short))
	{
		printf("SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
		return NULL;
	}

//	printf("size of packet received %d\n",*packetLen);

	*packetData=(Uint8*)malloc(*packetLen);
	if(!(*packetData))
	{
		return NULL;
	}

	if(*packetLen > 0)
	{
		result=SDLNet_TCP_Recv(sock,*packetData,*packetLen);
		if(result<*packetLen)
		{
			printf("SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
			return NULL;
		}
	}
//	printf("packet received\n");
	return *msgType;	
}



#endif
