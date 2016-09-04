//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/network/networkmanager.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/debug/debugvariable.hpp"
#include "common/engine/network/networkserver.hpp"

#if defined PLAT_IOS
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#endif

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////CVARBool( "Network", gServer, false );
//////CVARBool( "Network", gClient, false );
//////CVARBool( "Network", gSend, false );
//////CVARBool( "Network", gDisconnect, false );

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

NetworkManager::NetworkManager()
:	mNetworkServer( NULL )
{
}

//===========================================================================

NetworkManager::~NetworkManager()
{	
//////#if defined PLAT_PC || defined PLAT_IOS
//////
//////	Disconnect();
//////
//////	List<NetworkFunctor*>::Iterator iter = mNetworkListerners.CreateIterator();
//////	while ( iter.HasNext() )
//////	{
//////		NetworkFunctor* functor = iter.Next();
//////		DELETE_PTR( functor );
//////	}
//////
//////#endif
}

//===========================================================================

void NetworkManager::Initialize()
{	
	//mNetworkServer = NEW_PTR( "Network Server" ) NetworkServer;
	//mNetworkServer->Initialize();
}

//===========================================================================

//////void NetworkManager::SetupSocket()
//////{
//////#if defined PLAT_PC || defined PLAT_IOS
//////
//////	const StartupData* startupData = Database::Get()->GetStartupData();
//////	mPort = startupData->mPort;
//////	System::StringCopy( mIPAddress, 32, startupData->mIPAddress );
//////
//////	int error = 0;
//////	
//////#if defined PLAT_PC
//////	WSADATA w;
//////
//////	error = WSAStartup( 0x0202, &w );
//////	Assert( error == 0, "" );
//////#endif
//////	
//////	char name[255];
//////	error = gethostname( name, sizeof( name ) );
//////	Assert( error == 0, "" );
//////
//////	hostent* hostinfo;
//////	hostinfo = gethostbyname( name );
//////	Assert( hostinfo != NULL, "" );
//////
//////	char* ipAddress = inet_ntoa( *(struct in_addr *)*hostinfo->h_addr_list );
//////	UNUSED_ALWAYS( ipAddress );
//////	
//////	mSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
//////	//Assert( mSocket == 0, "" );
//////
//////	int flag = 1;
//////	int result = setsockopt( mSocket, IPPROTO_TCP, TCP_NODELAY, ( char* ) &flag,  sizeof( int ) );
//////	Assert( result == 0, "" );
//////
//////#endif
//////
//////}

//===========================================================================

//////void NetworkManager::Disconnect()
//////{
//////#if defined PLAT_PC || defined PLAT_IOS
//////	if ( mSocket != NULL )
//////	{
//////		int error = 0;
//////#if defined PLAT_IOS
//////		error = close( mSocket );
//////#endif
//////#if defined PLAT_PC
//////		error = closesocket( mSocket );
//////#endif
//////		Assert( error == 0, "" );
//////		mSocket = NULL;
//////	}
//////
//////	if ( mClient != NULL )
//////	{
//////		int error = 0;
//////#if defined PLAT_IOS
//////		error = close( mClient );
//////#endif
//////#if defined PLAT_PC
//////		error = closesocket( mClient );
//////#endif
//////		Assert( error == 0, "" );
//////		mClient = NULL;
//////	}
//////#endif
//////}

//===========================================================================

void NetworkManager::Shutdown()
{
	//mNetworkServer->Shutdown();
	//DELETE_PTR( mNetworkServer );
}

//===========================================================================

void NetworkManager::Update()
{
	//Assert( mNetworkServer != NULL, "" );
	//mNetworkServer->Update();
//////#if defined PLAT_PC || defined PLAT_IOS
//////	if ( gServer )
//////	{
//////		SetupSocket();
//////
//////		mNetworkType = NETWORK_SERVER;
//////
//////		sockaddr_in address;
//////		address.sin_family = AF_INET;   
//////		address.sin_port = htons( static_cast<unsigned short>( mPort ) );
//////		address.sin_addr.s_addr = htonl( INADDR_ANY );
//////
//////		int error = bind( mSocket, ( struct sockaddr* )&address, sizeof( struct sockaddr ) );
//////		Assert( error == 0, "" );
//////
//////		error = listen( mSocket, 5 );
//////		Assert( error == 0, "" );
//////
//////#if defined PLAT_PC
//////		unsigned long nonBlock = 1;
//////		ioctlsocket( mSocket, FIONBIO, &nonBlock );
//////#endif
//////		
//////#if defined PLAT_IOS
//////		 fcntl( mSocket, F_SETFL, O_NONBLOCK );
//////#endif
//////		gServer.SetBool( false );
//////	}
//////
//////	if ( gClient )
//////	{
//////		SetupSocket();
//////
//////		mNetworkType = NETWORK_CLIENT;
//////
//////		sockaddr_in target;
//////		target.sin_family = AF_INET;
//////		target.sin_port = htons( static_cast<unsigned short>( mPort ) );
//////		target.sin_addr.s_addr = inet_addr( mIPAddress ); 
//////		connect( mSocket, ( const sockaddr* )&target, sizeof( target ) );
//////
//////#if defined PLAT_PC
//////		unsigned long nonBlock = 1;
//////		ioctlsocket( mSocket, FIONBIO, &nonBlock );
//////#endif
//////		
//////#if defined PLAT_IOS
//////		fcntl( mSocket, F_SETFL, O_NONBLOCK );
//////#endif
//////
//////		gClient.SetBool( false );
//////	}
//////
//////	if ( gDisconnect )
//////	{
//////		Disconnect();
//////
//////		gDisconnect.SetBool( false );
//////	}
//////
//////	switch ( mNetworkType )
//////	{
//////	case NETWORK_SERVER:
//////		{
//////			sockaddr_in address;
//////			int size;
//////			size = sizeof( sockaddr_in );
//////
//////#if defined PLAT_IOS
//////			int newsock = accept( mSocket, ( sockaddr* )&address, ( socklen_t* )&size );			
//////#endif
//////#if defined PLAT_PC
//////			int newsock = accept( mSocket, ( sockaddr* )&address, ( int* )&size );			
//////#endif
//////
//////			if ( newsock != -1 )
//////			{
//////				int port = static_cast<int>( ntohs( address.sin_port ) );
//////				char* ipAddress = inet_ntoa( address.sin_addr );
//////
//////				mClient = newsock;
//////				System::PrintToConsole( "sock: %i, %i, %s\n", newsock, port, ipAddress );
//////			}
//////		}
//////		break;
//////	case NETWORK_CLIENT:
//////		{			
//////			char buffer[64];
//////			int bytesRecv = recv( mSocket, buffer, sizeof( buffer ), 0 );
//////			
//////			if ( bytesRecv > 0 )
//////			{
//////				char* tempBuffer = buffer;				
//////				System::PrintToConsole( "recv bytes: %i\n", bytesRecv );
//////
//////				int id = -1;
//////				memcpy( &id, tempBuffer, sizeof( int ) );
//////				tempBuffer += sizeof( int );
//////
//////				if ( id == 5 )
//////				{
//////					float x = 0;
//////					float y = 0;
//////					memcpy( &x, tempBuffer, sizeof( float ) );
//////					tempBuffer += sizeof( float );
//////					memcpy( &y, tempBuffer, sizeof( float ) );
//////
//////					System::PrintToConsole( "x: %f, y: %f, id: %i\n", x, y, id );
//////
//////					mX = x;
//////					mY = y;
//////
//////					List<NetworkFunctor*>::Iterator iter = mNetworkListerners.CreateIterator();
//////					while ( iter.HasNext() )
//////					{
//////						NetworkFunctor* functor = iter.Next();
//////						functor->Call( this );
//////					}
//////				}				
//////			}
//////		}
//////		break;
//////	}
//////#endif
}

//===========================================================================

//////void NetworkManager::SendData( float x, float y )
//////{
//////#if defined PLAT_PC || defined PLAT_IOS
//////	if ( mClient != NULL )
//////	{
//////		char buffer[64];
//////		char* tempBuffer = buffer;
//////		int id = 5;
//////		memcpy( tempBuffer, &id, sizeof( int ) );
//////		tempBuffer += sizeof( int );
//////		memcpy( tempBuffer, &x, sizeof( float ) );
//////		tempBuffer += sizeof( float );
//////		memcpy( tempBuffer, &y, sizeof( float ) );		
//////		int error = send( mClient, buffer, 64 * sizeof( char ), 0 );
//////		UNUSED_ALWAYS( error );
//////		
//////#if defined PLAT_PC
//////		int lasterror = WSAGetLastError();
//////		System::PrintToConsole( "lasterror: %i\n", lasterror );		
//////#endif
//////		
//////		if ( error == -1 )
//////		{
//////			if ( mClient != NULL )
//////			{
//////				int error = 0;
//////#if defined PLAT_IOS
//////				error = close( mClient );
//////#endif
//////#if defined PLAT_PC
//////				error = closesocket( mClient );
//////#endif
//////				Assert( error == 0, "" );
//////				mClient = NULL;
//////			}
//////			System::PrintToConsole( "Disconnect client.\n" );
//////		}
//////		//System::PrintToConsole( "x: %f, y: %f\n", x, y );
//////	}
//////#endif
//////}