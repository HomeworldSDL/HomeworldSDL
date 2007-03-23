#include <string.h>
#include <stdio.h>
#include "SDL.h"
#include "SDL_thread.h"
#ifdef HW_ENABLE_NETWORK
#include "SDL_net.h"
#endif
#include "NetworkInterface.h"

#ifdef HW_ENABLE_NETWORK
SDL_Thread *listenBroadcast;
static UDPsocket broadcastSendSock, broadcastRecvSock;
static int endNetwork;

void initNetwork()
{
	Uint32 sdl_flags;
	IPaddress ip, broadcastIp;
	broadcastIp.host = INADDR_BROADCAST;
	broadcastIp.port = UDPPORT;
	endNetwork = 1;

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

	// Resolve the argument into an IPaddress type
	if(SDLNet_ResolveHost(&ip,NULL,TCPPORT)==-1)
	{
		exit(2);
	}

	if(!(broadcastRecvSock = SDLNet_UDP_Open(UDPPORT)))
	{
		exit(3);
	}

	// open udp client socket
	if(!(broadcastSendSock=SDLNet_UDP_Open(0)))
	{
		exit(5);
	}

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
			printf("Packet received, length: %d id: %s port: %d\n", packet->len, packet->data,packet->address.port);
			ipaddr=SDL_SwapBE32(packet->address.host);
			printf("IP Address : %d.%d.%d.%d\n",
						ipaddr>>24,
						(ipaddr>>16)&0xff,
						(ipaddr>>8)&0xff,
						ipaddr&0xff);

			SDLNet_ResolveHost(&newIp,"255.255.255.255",UDPPORT);
			newIp.host=packet->address.host;
			if(checkList(newIp, listIps) == -1)
			{
				listIps = addList(newIp, listIps);
				if((number=SDLNet_UDP_Bind(data,0,&newIp))==-1)
				{
					printf("error binding\n");
				}
				else
					printf("binding last ip received, channel %d\n",number);
			}
			printf("packet recu taille : %d\n",packet->len);
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
	printf("not found in the list\n");

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
#endif
