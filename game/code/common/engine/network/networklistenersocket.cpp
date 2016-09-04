#if defined PLAT_PC

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/network/networklistenersocket.hpp"
#include "common/engine/network/serversocketmanager.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/network/packet.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

#ifndef PLAT_IOS

NetworkListenerSocket::NetworkListenerSocket( ServerSocketManager* serverSocketManager )
:	mPortNumber( -1 )
,	mServerSocketManager( serverSocketManager )
,	mOnAcceptConnectionCallback( NULL )
{
}

//===========================================================================

NetworkListenerSocket::~NetworkListenerSocket()
{
	DELETE_PTR( mOnAcceptConnectionCallback );
}

//===========================================================================

void NetworkListenerSocket::Initialize( int portNumber )
{
#if !defined PLAT_PS3
	struct sockaddr_in socketAddressIn;
	int returnValue = 0;

	//
	// Create socket handle
	//
	mSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if ( mSocket == INVALID_SOCKET )
	{
		Assert( false, "" );
	}

	//
	// Set socket options to reuse server socket addresses even if they are
	// busy - this is important if your server restarts and you don't want
	// to wait for your sockets to time out.
	//

	int optValue = 1;
	returnValue = setsockopt( mSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&optValue, sizeof( optValue ) );
	if ( returnValue == SOCKET_ERROR )
	{
		closesocket( mSocket );
		mSocket = INVALID_SOCKET;
		Assert( false, "" );
	}

	memset( &socketAddressIn, 0, sizeof( socketAddressIn ) );
	socketAddressIn.sin_family = AF_INET;
	socketAddressIn.sin_port = htons( (u_short)portNumber );

	//
	// Bind to port
	//
	returnValue = bind( mSocket, (struct sockaddr *)&socketAddressIn, sizeof( socketAddressIn ) );
	if ( returnValue == SOCKET_ERROR )
	{
		closesocket( mSocket );
		mSocket = INVALID_SOCKET;
		Assert( false, "" );
	}

	//
	// Set nonblocking - accept() blocks under some odd circumstances otherwise
	//
	const int block = 0;
	SetBlocking( block );

	//
	// Start listening
	//
	returnValue = listen( mSocket, 256 );
	if ( returnValue == SOCKET_ERROR )
	{
		closesocket( mSocket );
		mSocket = INVALID_SOCKET;
		Assert( false, "" );
	}

	mPortNumber = portNumber;
#endif
}

//===========================================================================

SOCKET NetworkListenerSocket::AcceptConnection( struct sockaddr_in &socketAddressIn )
{
#if !defined PLAT_PS3
	int size = sizeof( socketAddressIn );
	SOCKET newSocket = accept( mSocket, (struct sockaddr *)&socketAddressIn, &size );
	if ( newSocket == INVALID_SOCKET )
	{
		return INVALID_SOCKET;
	}

	int returnValue = getpeername( newSocket, (struct sockaddr *)&socketAddressIn, &size );
	if ( returnValue == SOCKET_ERROR )
	{
		closesocket( newSocket );
		return INVALID_SOCKET;
	}

	return newSocket;
#else
	return -1;
#endif
}

//===========================================================================

void NetworkListenerSocket::HandleInput()
{
#if !defined PLAT_PS3
	SOCKET newSocket;
	struct sockaddr_in socketAddressIn;
	newSocket = AcceptConnection( socketAddressIn );

	if ( newSocket != INVALID_SOCKET )
	{
		mOnAcceptConnectionCallback->Call( &newSocket );	
	}
#endif
}

#endif

#endif