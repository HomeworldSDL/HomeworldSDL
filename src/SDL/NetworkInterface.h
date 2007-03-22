#ifndef ___NETWORKINTERFACE_H
#define ___NETWORKINTERFACE_H

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#ifdef HW_ENABLE_NETWORK
#include <SDL/SDL_net.h>

typedef struct IpElemTmp { IPaddress IP; struct IpElemTmp *nextIP; } IpElem;

typedef IpElem* IpList;
		

void initNetwork();
void shutdownNetwork();

int broadcastStartThread(void*);
void sendBroadcastPacket(const void*, int);
int checkAndAdd(IPaddress, IpList);
int checkList(IPaddress, IpList);
IpList addList(IPaddress, IpList);

#define TCPPORT 10500
#define UDPPORT 10600

#endif
#endif
