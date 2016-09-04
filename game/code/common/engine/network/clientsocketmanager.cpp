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
#include "common/engine/network/clientsocketmanager.hpp"
#include "common/engine/network/networksocket.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/stringutilities.hpp"
#include "common/engine/system/assert.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

#ifndef PLAT_IOS

ClientSocketManager::ClientSocketManager( const char* hostName, int portNumber )
{
	System::StringCopy( mHostName, 256, hostName );
	mPortNumber = portNumber;
	mOnCreateGameSocketCallback = NULL;
}

//===========================================================================

ClientSocketManager::~ClientSocketManager()
{
}

//===========================================================================

bool ClientSocketManager::Connect()
{
	Assert( mOnCreateGameSocketCallback != NULL, "" );

	NetworkSocket* newNetworkSocket = mOnCreateGameSocketCallback( static_cast<SOCKET>( SOCKET_ERROR ) );
	bool isSuccess = newNetworkSocket->Connect( GetHostByName( mHostName ), mPortNumber );
	if ( isSuccess == false )
	{		
		return false;
	}

	AddSocket( newNetworkSocket );

	return true;
}

#endif

#endif