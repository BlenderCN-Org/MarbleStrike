#ifndef NETWORK_SOCKET_MANAGER_HPP
#define NETWORK_SOCKET_MANAGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <list>
#include <map>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class NetworkSocket;
class Packet;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class NetworkSocketManager
{
public:	

	NetworkSocketManager();
	~NetworkSocketManager();

	int AddSocket( NetworkSocket* socket );
	void RemoveSocket( NetworkSocket* socket );
	NetworkSocket* FindSocket( int socketID );
	bool Send( int socketID, Packet* packet );
	unsigned int GetHostByName( const char* hostName );
	void DoSelect( int pauseMicroSecs );

protected:

	typedef std::list< NetworkSocket* > NetworkSocketList;
	NetworkSocketList mSocketList;

	typedef std::map< int, NetworkSocket* > NetworkSocketMap;
	NetworkSocketMap mSocketIDMap;

	int mNumSocketConnections;
	int mNextSocketID;
};

#endif