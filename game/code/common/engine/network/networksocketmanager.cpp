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
#include "common/engine/network/networksocketmanager.hpp"
#include "common/engine/network/networksocket.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// WARNING DISABLE
//////////////////////////////////////////////////////

//
// Necessary for FD_SET and other functions in winsock
//
#pragma warning( push )
#pragma warning(disable:4127) // Disable "conditional expression is constant" warning

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

#ifndef PLAT_IOS

NetworkSocketManager::NetworkSocketManager()
:	mNumSocketConnections( 0 )
,	mNextSocketID( 0 )
{
#if !defined PLAT_PS3
	WSADATA wsaData;

	int errorCode = WSAStartup( 0x0202, &wsaData );	
	if ( errorCode != 0 )
	{
		Assert( false, "WSAStartup failure!" );		
	}
#endif
}

//===========================================================================

NetworkSocketManager::~NetworkSocketManager()
{
#if !defined PLAT_PS3
	while ( !mSocketList.empty() )
	{
		DELETE_PTR( *mSocketList.begin() );
		mSocketList.pop_front();
	}

	WSACleanup();
#endif
}

//===========================================================================

int NetworkSocketManager::AddSocket( NetworkSocket* socket )
{
	socket->SetID( mNextSocketID );
	mSocketList.push_back( socket );

	int openSockets = static_cast<int>( mSocketList.size() );
	if ( openSockets > mNumSocketConnections )
	{
		++mNumSocketConnections;
	}

	mSocketIDMap[mNextSocketID] = socket;
	++mNextSocketID;

	return socket->GetID();
}

//===========================================================================

void NetworkSocketManager::RemoveSocket( NetworkSocket* socket )
{
	mSocketList.remove( socket );
	mSocketIDMap.erase( socket->GetID() );
}

//===========================================================================

NetworkSocket* NetworkSocketManager::FindSocket( int socketID )
{
	std::map< int, NetworkSocket* >::iterator iter = mSocketIDMap.find( socketID );
	if ( iter == mSocketIDMap.end() )
	{
		return NULL;
	}

	return ( *iter ).second;
}

//===========================================================================

bool NetworkSocketManager::Send( int socketID, Packet* packet )
{
	NetworkSocket* socket = FindSocket( socketID );
	if ( !socket )
	{
		return false;
	}

	socket->Send( packet );

	return true;
}

//===========================================================================

unsigned int NetworkSocketManager::GetHostByName( const char* hostName )
{
#if defined PLAT_PC
	struct hostent* hostEnt;
	struct sockaddr_in tempSockAddr; //placeholder for the ip address

	//
	// This will retrieve the ip details and put it into hostEnt structure
	//
	hostEnt = gethostbyname( hostName );

	if ( hostEnt == NULL )
	{
		Assert( false, "" );
		return 0;
	}

	memcpy( &tempSockAddr.sin_addr, hostEnt->h_addr, hostEnt->h_length );
	unsigned long value = ntohl( tempSockAddr.sin_addr.s_addr );
	return value;
#else
	return 0;
#endif
}

//===========================================================================

void NetworkSocketManager::DoSelect( int pauseMicroSecs )
{
#if !defined PLAT_PS3
	fd_set inp_set;
	fd_set out_set;
	fd_set exc_set;	
	FD_ZERO( &inp_set );
	FD_ZERO( &out_set );
	FD_ZERO( &exc_set );

	//
	// Set everything up for the select
	//
	int maxDescriptor = 0;
	for ( NetworkSocketList::iterator i = mSocketList.begin(); i != mSocketList.end(); ++i )
	{
		 NetworkSocket* networkSocket = *i;
		 SOCKET socket = networkSocket->GetSocket();
	//	if ((pSock->m_deleteFlag&1) || pSock->m_sock == INVALID_SOCKET)
	//		continue;

	//	if (handleInput)
		FD_SET( socket, &inp_set );

		FD_SET( socket, &exc_set );

		if ( networkSocket->HasOutput() )
		{
			FD_SET( socket, &out_set );
		}

		if ( (int)socket > maxDescriptor )
		{
			maxDescriptor = (int)socket;
		}
	}
	

	//
	// Do the select (duration passed in as tv, NULL to block until event)
	//

	timeval tv;
	tv.tv_sec = 0;
	// 100 microseconds is 0.1 milliseconds or .0001 seconds
	tv.tv_usec = pauseMicroSecs;

	int selectReturn = 0;
	selectReturn = select( maxDescriptor + 1, &inp_set, &out_set, &exc_set, &tv );
	if ( selectReturn == SOCKET_ERROR )
	{
		int errorCode = WSAGetLastError();
		System::PrintToConsole( "%i\n", errorCode );
		Assert( false, "Network Select Error." );
		return;
	}	
	else
	{
		//
		// Handle input, output, and exceptions
		//

		for ( NetworkSocketList::iterator i = mSocketList.begin(); i != mSocketList.end(); ++i )
		{
			NetworkSocket* networkSocket = *i;
			SOCKET socket = networkSocket->GetSocket();

	//		if ((pSock->m_deleteFlag&1) || pSock->m_sock == INVALID_SOCKET)
	//			continue;

			if ( FD_ISSET( socket, &exc_set ) )
			{
				networkSocket->HandleException();
			}

	//		if (!(pSock->m_deleteFlag&1) && FD_ISSET(pSock->m_sock, &out_set))
			if ( FD_ISSET( socket, &out_set ) )
			{
				networkSocket->HandleOutput();
			}

	//		if ( handleInput && !(pSock->m_deleteFlag&1) && FD_ISSET(pSock->m_sock, &inp_set))
			if ( FD_ISSET( socket, &inp_set ) )
			{
				networkSocket->HandleInput();
			}
		}
	}

	//unsigned int timeNow = timeGetTime();

	//// handle deleting any sockets
	//for (SocketList::iterator i = m_SockList.begin();
	//	i != m_SockList.end(); ++i)
	//{
	//	pSock = *i;
	//	if (pSock->m_timeOut)
	//	{
	//		if (pSock->m_timeOut < timeNow)
	//			pSock->TimeOut();
	//	}

	//	if (pSock->m_deleteFlag&1)
	//	{
	//		switch (pSock->m_deleteFlag)
	//		{
	//		case 1:
	//			--i;
	//			g_pSocketManager->RemoveSocket(pSock);
	//			break;
	//		case 3:
	//			pSock->m_deleteFlag = 2;
	//			if (pSock->m_sock != INVALID_SOCKET)
	//			{
	//				closesocket(pSock->m_sock);
	//				pSock->m_sock = INVALID_SOCKET;
	//			}
	//			break;
	//		}
	//	}
	//}
#endif
}

#endif

#pragma warning( pop )
#endif