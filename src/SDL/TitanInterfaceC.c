/*============================================================================
 * TitanInterfaceC.stub.c
 * Dummy functions to simulate a WONnet connection that never works.
 *
 * Author:  Ted Cipicchio <ted@thereisnospork.com>
 * Created: Sat Oct 4 2003
 *==========================================================================*/
#include "TitanInterfaceC.h"
#include "NetworkInterface.h"

/*----------------------------------------------------------------------------
 * Global Variables
 *--------------------------------------------------------------------------*/

wchar_t ChannelPassword[MAX_PASSWORD_LEN];
TPChannelList tpChannelList;
TPServerList tpServerList;
CaptainGameInfo tpGameCreated;
Address myAddress;
unsigned long DIRSERVER_PORTS[MAX_PORTS];
unsigned long PATCHSERVER_PORTS[MAX_PORTS];
ipString DIRSERVER_IPSTRINGS[MAX_IPS];
ipString PATCHSERVER_IPSTRINGS[MAX_IPS];
unsigned long HomeworldCRC[4];
wchar_t GameWereInterestedIn[MAX_TITAN_GAME_NAME_LEN];
void *GameWereInterestedInMutex = 0;


/*----------------------------------------------------------------------------
 * Functions
 *--------------------------------------------------------------------------*/

void titanGotNumUsersInRoomCB(const wchar_t *theRoomName, int theNumUsers)
{
	dbgMessagef("\ntitanGotNumUsersInRoomCB");
}


unsigned long titanStart(unsigned long isLan, unsigned long isIP)
{
	dbgMessagef("\ntitanStart");
#ifdef HW_ENABLE_NETWORK
	initNetwork();
	myAddress.AddrPart.IP = getMyAddress();
	myAddress.Port = TCPPORT;
	return 1; 
#else
	return 0;
#endif
}


unsigned long titanCheckCanNetwork(unsigned long isLan, unsigned long isIP)
{
	dbgMessagef("\ntitanCheckCanNetwork");
	if ( isLan != 0 && isIP == 1)
		return 1;
	else
		return 0;
}


// --MikeN
// Call this method to begin shutdown of titan.  Parameters specify packet to send
// to connected client(s) (a shutdown message).  The callback titanNoClientsCB() will
// be invoked when complete.
void titanStartShutdown(unsigned long titanMsgType, const void* thePacket,
                        unsigned short theLen)
{
	dbgMessagef("\ntitanStartShutdown");
	/* Probably won't ever be called, but we'll be consistent anyway... */
	titanNoClientsCB();
}


void titanLeaveGameNotify(void)
{
	dbgMessagef("\ntitanLeaveGameNotify");
}


void titanShutdown(void)
{
#ifdef HW_ENABLE_NETWORK
	shutdownNetwork();
#endif
	dbgMessagef("\ntitanShutdown");
}


void titanRefreshRequest(char* theDir)
{
	dbgMessagef("\ntitanRefreshRequest");
}


unsigned long titanReadyToStartGame(unsigned char *routingaddress)
{
	dbgMessagef("\ntitanReadyToStartGame");
	return 0; 
}


unsigned long titanBehindFirewall(void)
{
	dbgMessagef("\ntitanBehindFirewall");
	return 0; 
}


void titanCreateGame(wchar_t *str, DirectoryCustomInfo* myInfo)
{
	dbgMessagef("\ntitanCreateGame");

}


void titanRemoveGame(wchar_t *str)
{
	dbgMessagef("\ntitanRemoveGame");

}


void titanCreateDirectory(char *str, char* desc)
{
	dbgMessagef("\ntitanCreateDirectory");

}


void titanSendLanBroadcast(const void* thePacket, unsigned short theLen)
{
	dbgMessagef("\ntitanSendLanBroadcast");
#ifdef HW_ENABLE_NETWORK
	sendBroadcastPacket(thePacket, theLen);
#endif
//	titanReceivedLanBroadcastCB(thePacket, theLen);
}


void titanSendPacketTo(Address *address, unsigned char titanMsgType,
                       const void* thePacket, unsigned short theLen)
{
	dbgMessagef("\ntitanSendPacketTo");

}


void titanBroadcastPacket(unsigned char titanMsgType, const void* thePacket, unsigned short theLen)
{
	dbgMessagef("\ntitanBroadcastPacket");

}


void titanAnyoneSendPacketTo(Address *address, unsigned char titanMsgType,
                       const void* thePacket, unsigned short theLen)
{
	dbgMessagef("\ntitanAnyoneSendPacketTo");

}


void titanAnyoneBroadcastPacket(unsigned char titanMsgType, const void* thePacket, unsigned short theLen)
{
	dbgMessagef("\ntitanAnyoneBroadcastPacket");

}


void titanConnectToClient(Address *address)
{
	connectToServer(address->AddrPart.IP);
}


int titanStartChatServer(wchar_t *password)
{
	dbgMessagef("\ntitanStartChatServer");
	return 0;
}


void titanSendPing(Address *address,unsigned int pingsizebytes)
{
	dbgMessagef("\ntitanSendPing");
}


void titanPumpEngine()
{
	dbgMessagef("\ntitanPumpEngine");
}


void titanSetGameKey(unsigned char *key)
{
	dbgMessagef("\ntitanSetGameKey");
}


const unsigned char *titanGetGameKey(void)
{
	dbgMessagef("\ntitanGetGameKey");
	return 0; 
}


Address titanGetMyPingAddress(void)
{ 
	dbgMessagef("\ntitanGetMyPingAddress");
	return myAddress; 
}


int titanGetPatch(char *filename,char *saveFileName)
{
	dbgMessagef("\ntitanGetPatch");
	titanGetPatchFailedCB(PATCHFAIL_UNABLE_TO_CONNECT);
	return PATCHFAIL_UNABLE_TO_CONNECT;
}


void titanReplaceGameInfo(wchar_t *str, DirectoryCustomInfo* myInfo, unsigned long replaceTimeout)
{
	dbgMessagef("\ntitanReplaceGameInfo");
}


void chatConnect(wchar_t *password)
{
	dbgMessagef("\nchatConnect");
}


void chatClose(void)
{
	dbgMessagef("\nchatClose");
}


void BroadcastChatMessage(unsigned short size, const void* chatData)
{
	dbgMessagef("\nBroadcastChatMessage");
}


void SendPrivateChatMessage(unsigned long* userIDList, unsigned short numUsersInList,
                            unsigned short size, const void* chatData)
{
	dbgMessagef("\nSendPrivateChatMessage");
}


void authAuthenticate(char *loginName, char *password)
{
	dbgMessagef("\nauthAuthenticate");
}


void authCreateUser(char *loginName, char *password)
{
	dbgMessagef("\nauthCreateUser");
}


void authChangePassword(char *loginName, char *oldpassword, char *newpassword)
{
	dbgMessagef("\nauthChangePassword");
}


int titanSaveWonstuff()
{ 
	dbgMessagef("\ntitanSaveWonstuff");
	return 0; 
}


void titanWaitShutdown(void)
{
	dbgMessagef("\ntitanWaitShutdown");
}


void titanConnectingCancelHit(void)
{
	dbgMessagef("\ntitanConnectingCancelHit");
}

#ifdef HW_ENABLE_NETWORK
void HandleTCPMessage()
{

}

#endif
