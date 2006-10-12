// =============================================================================
//  Chatting.c
//  - handles all chat data and makes call backs as neccessary
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
// =============================================================================

#include "Chatting.h"

#include "Alliance.h"
#include "CommandNetwork.h"
#include "GameChat.h"
#include "HorseRace.h"
#include "MultiplayerGame.h"
#include "MultiplayerLANGame.h"


void recievedChatPacketCB(ubyte *packet,udword sizeofPacket)
{
#define chatpacket ((ChatPacket *)packet)
    switch (chatpacket->messageType)
    {
        case CHAT_PACKET:
        {
            // convert chat packet wide string to normal string:
            char aNarrowString[MAX_CHAT_MESSAGE_LENGTH];
            wcstombs(aNarrowString,(const wchar_t *)&chatpacket->message[0],MAX_CHAT_MESSAGE_LENGTH);
            strcpy(chatpacket->message,aNarrowString);

            if (hrRunning)
            {
                hrProcessPacket(chatpacket);
            }
            else if (mgRunning)
            {
                mgProcessGameChatPacket(chatpacket);
            }
            else if (lgRunning)
            {
                lgProcessGameChatPacket(chatpacket);
            }
            else if (gcRunning)
            {
                gcProcessGameChatPacket(chatpacket);
            }
        }
            break;
        case ALLIANCE_PACKET:
            allianceFormRequestRecievedCB(chatpacket);
            break;
    }
#undef chatpacket
}

void sendChatMessage(udword users, char *message, uword player_index)
{
    ChatPacket packetPtr;
    udword  stringlength = strlen(message);
    udword sizeofpacket;
    wchar_t aWideStringP[MAX_CHAT_MESSAGE_LENGTH];
    size_t  aNumChars;

    dbgAssert(stringlength < MAX_CHAT_MESSAGE_LENGTH);
    dbgAssert(message[stringlength]==0);

    // now convert message, stringlength to widestring:

    aNumChars = mbstowcs(aWideStringP, message, stringlength+1);
    if (aNumChars != (size_t)-1)
    {
        aNumChars++;
        sizeofpacket = (sizeof(ChatPacket) - MAX_CHAT_MESSAGE_LENGTH*sizeof(wchar_t)) + (aNumChars<<1);

        //contruct packet by concatinating the message and the actual packet header
        packetPtr.packetheader.type = PACKETTYPE_CHAT;
        packetPtr.packetheader.frame = player_index;
        memcpy( packetPtr.message, aWideStringP, aNumChars<<1);
        packetPtr.users = (uword)users;
        packetPtr.messageType = CHAT_PACKET;

        SendChatPacketPacket(&packetPtr, sizeofpacket,users);
    }
}

void sendAllianceRequest(udword user, uword player_index, uword type, ubyte data)
{
    ChatPacket packetPtr;
    udword sizeofpacket;

    sizeofpacket = (sizeof(ChatPacket) - MAX_CHAT_MESSAGE_LENGTH);

    //contruct packet by concatinating the message and the actual packet header
    packetPtr.packetheader.type = PACKETTYPE_CHAT;
    packetPtr.packetheader.frame = player_index;
    packetPtr.users = (uword)user;
    packetPtr.message[0] = data;
    packetPtr.messageType = type;

    SendChatPacketPacket(&packetPtr, sizeofpacket,user);
}

