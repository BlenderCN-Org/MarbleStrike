#ifndef SERVER_SOCKET_MANAGER_HPP
#define SERVER_SOCKET_MANAGER_HPP

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
#include "common/engine/system/classcallback.hpp"
#include "common/engine/network/networklistenersocket.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

#ifndef PLAT_IOS

class ServerSocketManager : public NetworkSocketManager
{
public:	

	ServerSocketManager();
	~ServerSocketManager();

	void OnAcceptConnectionCallback( void* data );

	void SetCreateGameSocketCallback( NetworkSocket* (*onCreateGameSocketCallback) ( SOCKET ) )
	{
		mOnCreateGameSocketCallback = onCreateGameSocketCallback;
	}

private:

	NetworkListenerSocket* mNetworkListenerSocket;

	NetworkSocket* (*mOnCreateGameSocketCallback) ( SOCKET );
};

#endif

#endif