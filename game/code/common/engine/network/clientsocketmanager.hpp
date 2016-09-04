#ifndef CLIENT_SOCKET_MANAGER_HPP
#define CLIENT_SOCKET_MANAGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#if defined PLAT_PS3
#include "ps3/ps3defines.hpp"
#endif

#include "common/engine/network/networksocketmanager.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class ClientSocketManager : public NetworkSocketManager
{
public:	

	ClientSocketManager( const char* hostName, int portNumber );
	~ClientSocketManager();

	bool Connect();

#ifndef PLAT_IOS	
	void SetCreateGameSocketCallback( NetworkSocket* (*onCreateGameSocketCallback) ( SOCKET ) )
	{
		mOnCreateGameSocketCallback = onCreateGameSocketCallback;
	}
#endif

private:

	char mHostName[256];
	int mPortNumber;

#ifndef PLAT_IOS
	NetworkSocket* (*mOnCreateGameSocketCallback) ( SOCKET );
#endif
};

#endif