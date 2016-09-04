#if defined PLAT_PC

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
#include <netinet/in.h>
#include <string.h>
#endif
#include "common/engine/network/packet.hpp"
#include "common/engine/system/memory.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

#ifndef PLAT_IOS

Packet::Packet( char const * const data, int size )
{
	mPacketDataBuffer = NEW_PTR( "Packet Data" ) char[size + sizeof( int )];
	mPacketSize = size + sizeof( int );

	*( int* )mPacketDataBuffer = htonl( size );
	memcpy( mPacketDataBuffer + sizeof( int ), data, sizeof( char ) * size );	
}

//===========================================================================

Packet::~Packet()
{
	DELETE_PTR_ARRAY( mPacketDataBuffer );
}

//===========================================================================

char* Packet::GetPacketData()
{
	return mPacketDataBuffer;
}

//===========================================================================

int Packet::GetPacketSize()
{
	return mPacketSize;
}

//===========================================================================

char* Packet::UnpackPacketData()
{
	return ( mPacketDataBuffer + sizeof( int ) );
}

//===========================================================================

int Packet::UnpackPacketDataSize()
{
	return ( mPacketSize - sizeof( int ) );
}

#endif

#endif