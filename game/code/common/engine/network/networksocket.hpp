#ifndef NETWORK_SOCKET_HPP
#define NETWORK_SOCKET_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#if defined PLAT_PC
#include <winsock.h>
#endif
#if defined PLAT_XBOX360
#include <Xtl.h>
#endif
#if defined PLAT_PS3
#include "ps3/ps3defines.hpp"
#endif
#include <list>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class Packet;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

#define MAX_PACKET_SIZE (256)
#define RECV_BUFFER_SIZE (MAX_PACKET_SIZE * 512)

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

#ifndef PLAT_IOS

class NetworkSocket
{
public:	

	NetworkSocket();	
	NetworkSocket( SOCKET newSocket );
	~NetworkSocket();	
	
	bool Connect( unsigned int ip, unsigned int port );
	void SetBlocking( int block );

	void Send( Packet* packet );
	bool HasOutput();

	void HandleException();
	void HandleOutput();
	virtual void HandleInput();

	void SetID( int value )
	{
		mID = value;
	}

	int GetID()
	{
		return mID;
	}

	SOCKET GetSocket()
	{
		return mSocket;
	}
	
protected:

	typedef std::list< Packet* > PacketList;
	PacketList mOutBoundPacketList;
	PacketList mInBoundPacketList;

	char mReceiveBuffer[RECV_BUFFER_SIZE];

	SOCKET mSocket;
	int mID;	
};

#endif

#endif