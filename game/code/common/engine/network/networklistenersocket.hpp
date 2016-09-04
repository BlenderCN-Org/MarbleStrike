#ifndef NETWORK_LISTENER_SOCKET_HPP
#define NETWORK_LISTENER_SOCKET_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/network/networksocket.hpp"
#include "common/engine/system/classcallback.hpp"
#include "common/engine/system/memory.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class ServerSocketManager;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

#ifndef PLAT_IOS

class NetworkListenerSocket : public NetworkSocket
{
public:	

	NetworkListenerSocket( ServerSocketManager* serverSocketManager );
	~NetworkListenerSocket();

	void Initialize( int portNumber );
	SOCKET AcceptConnection( struct sockaddr_in &socketAddressIn );

	void HandleInput();

	template<typename TCLASS>
	void SetOnAcceptConnectionCallback( void ( TCLASS::*Callback )( void* ), TCLASS* that )
	{
		DELETE_PTR( mOnAcceptConnectionCallback );
		mOnAcceptConnectionCallback = NEW_PTR( "Create Network Socket" ) ClassCallbackSpecific<TCLASS>( that, Callback );		
	}	

private:	

	ServerSocketManager* mServerSocketManager;
	int mPortNumber;

	ClassCallback* mOnAcceptConnectionCallback;
	
};

#endif

#endif