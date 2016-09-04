#if defined PLAT_PC

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/network/serversocketmanager.hpp"
#include "common/engine/network/serversocketmanager.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

#ifndef PLAT_IOS

ServerSocketManager::ServerSocketManager()
{
	mNetworkListenerSocket = NEW_PTR( "Listener Socket" ) NetworkListenerSocket( this );
	mNetworkListenerSocket->Initialize( 7090 );
	mNetworkListenerSocket->SetOnAcceptConnectionCallback( &ServerSocketManager::OnAcceptConnectionCallback, this );
	AddSocket( mNetworkListenerSocket );
}

//===========================================================================

ServerSocketManager::~ServerSocketManager()
{
}

//===========================================================================

void ServerSocketManager::OnAcceptConnectionCallback( void* data )
{
	SOCKET newSocket = *( reinterpret_cast<SOCKET*>( data ) );

	if ( newSocket != INVALID_SOCKET )
	{
		NetworkSocket* newNetworkSocket = mOnCreateGameSocketCallback( newSocket );
		AddSocket( newNetworkSocket );
	}	
}

#endif

#endif