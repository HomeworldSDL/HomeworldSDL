#ifndef ___NETWORKINTERFACE_H
#define ___NETWORKINTERFACE_H

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#ifdef HW_ENABLE_NETWORK
#include <SDL/SDL_net.h>

typedef struct IpElemTmp { IPaddress IP; struct IpElemTmp *nextIP; } IpElem;

typedef IpElem* IpList;

typedef struct { TCPsocket sock; IPaddress IP; } Client;

void initNetwork();
void shutdownNetwork();

int broadcastStartThread(void*);
int TCPServerStartThread(void*);
void sendBroadcastPacket(const void*, int);
void connectToServer(Uint32);

Client * addSockToList(TCPsocket sock);
TCPsocket findSockInList(Uint32 addressSock);

int checkList(IPaddress, IpList);
IpList addList(IPaddress, IpList);

Uint32 getMyAddress();
int pingSendThread(void*);

// Callback to the function that Handle messages.
void HandleTCPMessage();


#define TCPPORT 10500
#define UDPPORT 10600

#endif
#endif
