#if defined PLAT_PC

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/network/networkserver.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/render/effectmanager.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/network/networkmessages.hpp"

#if defined PLAT_PC
#include <WinSock.h>
#endif

#if defined PLAT_IOS
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#endif

#if defined PLAT_XBOX360
#include <Xtl.h>
#endif

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

NetworkServer::NetworkServer()
:	mPort( 0 )
,	mSocket( NULL )
,	mClient( NULL )
{
	mIPAddress[0] = '\0';
}

//===========================================================================

NetworkServer::~NetworkServer()
{
}

//===========================================================================

void NetworkServer::Initialize()
{
#if defined PLAT_PC
	const StartupData* startupData = Database::Get()->GetStartupData();
	mPort = startupData->mPort;
	System::StringCopy( mIPAddress, 32, startupData->mIPAddress );

	int error = 0;

#if defined PLAT_PC
	WSADATA w;

	error = WSAStartup( 0x0202, &w );
	Assert( error == 0, "" );
#endif

	char name[255];
	error = gethostname( name, sizeof( name ) );
	Assert( error == 0, "" );

	hostent* hostinfo;
	hostinfo = gethostbyname( name );
	Assert( hostinfo != NULL, "" );

	char* ipAddress = inet_ntoa( *(struct in_addr *)*hostinfo->h_addr_list );
	UNUSED_ALWAYS( ipAddress );

	mSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	Assert( mSocket != INVALID_SOCKET, "" );

	int flag = 1;
	int result = setsockopt( mSocket, IPPROTO_TCP, TCP_NODELAY, ( char* ) &flag,  sizeof( int ) );
	result;
	Assert( result == 0, "" );

	sockaddr_in address;
	address.sin_family = AF_INET;   
	address.sin_port = htons( static_cast<unsigned short>( mPort ) );
	address.sin_addr.s_addr = htonl( INADDR_ANY );

	error = bind( mSocket, ( struct sockaddr* )&address, sizeof( struct sockaddr ) );
	Assert( error == 0, "" );

	const int maxNumPendingConnections = 5;
	error = listen( mSocket, maxNumPendingConnections );
	Assert( error == 0, "" );

#if defined PLAT_PC
	unsigned long nonBlock = 1;
	ioctlsocket( mSocket, FIONBIO, &nonBlock );
#endif

#if defined PLAT_IOS
	fcntl( mSocket, F_SETFL, O_NONBLOCK );
#endif
#endif 

#if defined PLAT_XBOX360

	XNetStartupParams xnsp;
	memset(&xnsp, 0, sizeof(xnsp));
	xnsp.cfgSizeOfStruct = sizeof(XNetStartupParams);
	xnsp.cfgFlags = XNET_STARTUP_BYPASS_SECURITY;

	if ( XNetStartup( &xnsp ) != 0 )
	{
		Assert( false, "XNetStartup failed.\n" );
	}

	WORD wVersion = MAKEWORD( 2, 2 );   // request version 2.2 of Winsock
	WSADATA wsaData;
	int err = WSAStartup( wVersion, &wsaData );
	if ( err != 0 )
	{
		System::PrintToConsole( "WSAStartup failed, error %d.\n", err );
		Assert( false, "" );
	}

	if ( wsaData.wVersion != wVersion )
	{
		System::PrintToConsole( "Failed to get proper version of Winsock, got %d.%d.\n",
			LOBYTE( wsaData.wVersion ), HIBYTE( wsaData.wVersion ) );
		Assert( false, "" );
	}

	mSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if ( mSocket == INVALID_SOCKET )
	{
		System::PrintToConsole( "Failed to create socket, error %d.\n", WSAGetLastError() );
		Assert( false, "" );
	}

	SOCKADDR_IN sa;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = INADDR_ANY;
	sa.sin_port = htons( 1000 );

	if ( bind( mSocket, ( const sockaddr* )&sa, sizeof( sa ) ) != 0 )
	{
		System::PrintToConsole( "Failed to bind socket, error %d.\n", WSAGetLastError() );
		Assert( false, "" );
	}

	const int maxNumPendingConnections = 5;
	int error = listen( mSocket, maxNumPendingConnections );
	Assert( error == 0, "" );

	unsigned long iUnblocking = 1;
	if( ioctlsocket( mSocket, FIONBIO, &iUnblocking ) != 0 )
	{
		System::PrintToConsole( "Failed to set socket to nonblocking, error %d\n", WSAGetLastError() );
		Assert( false, "" );
	}
#endif
}

//===========================================================================

void NetworkServer::Shutdown()
{
	ShutdownSocket( mSocket );
	ShutdownSocket( mClient );

#if defined PLAT_PC || defined PLAT_XBOX360
	WSACleanup();
#endif

#if defined PLAT_XBOX360
	XNetCleanup();
#endif
}

//===========================================================================

void NetworkServer::Update()
{
#if defined PLAT_PC || defined PlAT_XBOX360 || defined PLAT_IOS

	sockaddr_in address;
	int size;
	size = sizeof( sockaddr_in );

#if defined PLAT_IOS
	int newsock = accept( mSocket, ( sockaddr* )&address, ( socklen_t* )&size );			
#endif
#if defined PLAT_PC || defined PLAT_XBOX360
	int newsock = accept( mSocket, ( sockaddr* )&address, ( int* )&size );			
#endif

	if ( newsock != -1 )
	{		
#if defined PLAT_PC
		int port = static_cast<int>( ntohs( address.sin_port ) );
		char* ipAddress = inet_ntoa( address.sin_addr );
		System::PrintToConsole( "sock: %i, %i, %s\n", newsock, port, ipAddress );
#endif

		if ( mClient != 0 )
		{
			ShutdownSocket( mClient );
		}

		mClient = newsock;		
	}

	if ( mClient != 0 )
	{
		const int bufferLength = 512;
		char buffer[bufferLength];
		int bytesRecv = recv( mClient, buffer, bufferLength, 0 );

		if ( bytesRecv > 0 )
		{
			ProcessMessage( buffer, bytesRecv );	
		}

		int error = send( mClient, "ack", 3, 0 );
		//if ( error == SOCKET_ERROR )
		if ( error == 0 )
		{
			ShutdownSocket( mClient );
		}
	}

#endif
}

//===========================================================================

void NetworkServer::ShutdownSocket( int &socket )
{
	if ( socket != 0 )
	{
		int error = 0;

#if defined PLAT_IOS
		error = close( socket );
#endif
#if defined PLAT_PC || defined PLAT_XBOX360
		error = closesocket( socket );
#endif
		Assert( error == 0, "" );
		socket = NULL;
	}
}

//===========================================================================

void NetworkServer::ProcessMessage( char* buffer, int /*bytesReceived*/ )
{
	int messageID = 0;
	System::ReadInteger( buffer, messageID, true );

	if ( messageID >= 0 && messageID < NETWORK_MESSAGE_MAX )
	{
		switch ( messageID )
		{
		case NETWORK_ACKNOWLEDGE:
			break;
		case NETWORK_MESSAGE_SHADER_RELOAD:
			{
				EffectManager* effectManager = GameApp::Get()->GetEffectManager();
				effectManager->Reload();
			}			
			break;
		default:
			System::PrintToConsole( "Invalid Message ID: %i", messageID );
			break;
		}
	}	
	else
	{
		System::PrintToConsole( "Invalid Message ID: %i\n", messageID );
	}
}

#endif