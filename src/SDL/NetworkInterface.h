#ifndef ___NETWORKINTERFACE_H
#define ___NETWORKINTERFACE_H

#include "SDL.h"
#include "SDL_thread.h"
#ifdef HW_ENABLE_NETWORK
#include "SDL_net.h"

typedef struct IpElemTmp { IPaddress IP; struct IpElemTmp *nextIP; } IpElem;

typedef IpElem* IpList;

typedef struct { TCPsocket sock; IPaddress IP; } Client;

void initNetwork();
void shutdownNetwork();

int broadcastStartThread(void*);
int TCPServerStartThread(void*);
void sendBroadcastPacket(const void*, int);
Uint32 connectToServer(Uint32);

Client * addSockToList(TCPsocket);
TCPsocket findSockInList(Uint32);
void removeSockFromList(int);

int checkList(IPaddress, IpList);
IpList addList(IPaddress, IpList);

Uint32 getMyAddress();
int pingSendThread(void*);


// Function defined depending on the protocol used
void putPacket(Uint32, unsigned char, const void*, unsigned short);
unsigned char getPacket(TCPsocket, unsigned char*, Uint8**, unsigned short*);

// Callback to the function that Handle messages.
void HandleTCPMessage(Uint32, unsigned char, const void*, unsigned short);

void HandleJoinGame(Uint32, const void*, unsigned short);
void HandleJoinConfirm(Uint32, const void*, unsigned short);
void HandleGameData(const void*, unsigned short);
void HandleGameStart(const void*, unsigned short);
void HandleGameMsg(const void*, unsigned short);


#define TCPPORT 10500
#define UDPPORT 10600

#endif
#endif
