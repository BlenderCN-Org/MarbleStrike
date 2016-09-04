#ifndef NETWORK_MANAGER_HPP
#define NETWORK_MANAGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#if defined PLAT_PC
#include <WinSock.h>
#endif

#if defined PLAT_IOS
#include <netinet/in.h>
#include <netinet/tcp.h>
#endif

//////#include "common/engine/system/list.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class NetworkServer;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

//////class NetworkFunctor
//////{
//////public:
//////
//////	virtual void operator()( NetworkManager* ) = 0;
//////	virtual void Call( NetworkManager* ) = 0;
//////};
//////
////////===========================================================================
//////
//////template <class TClass> 
//////class NetworkSpecificFunctor : public NetworkFunctor
//////{
//////public:
//////
//////	NetworkSpecificFunctor( TClass* object, void( TClass::*callback )( NetworkManager* ) )
//////	{ 
//////		mObject = object;  
//////		mCallback = callback; 
//////	}
//////
//////	virtual void operator()( NetworkManager* networkManager )
//////	{ 
//////		( *mObject.*mCallback )( networkManager );
//////	}
//////
//////	virtual void Call( NetworkManager* networkManager )
//////	{ 
//////		( *mObject.*mCallback )( networkManager );
//////	}
//////
//////private:
//////	void ( TClass::*mCallback )( NetworkManager* );
//////	TClass* mObject;
//////};

//===========================================================================

class NetworkManager
{
public:	

	//////enum NetworkType
	//////{
	//////	NETWORK_SERVER,
	//////	NETWORK_CLIENT,
	//////	NETWORK_NONE
	//////};

	NetworkManager();
	~NetworkManager();

	void Initialize();
	void Update();
	void Shutdown();
	//////void SetupSocket();
	//////void Disconnect();
	

	//////void SendData( float x, float y );

	//////template<typename TCLASS>
	//////void RegisterListener( void ( TCLASS::*Callback )( NetworkManager* ), TCLASS* that )
	//////{		
	//////	NetworkFunctor* newFunctor = NEW_PTR( "Network Functor" ) NetworkSpecificFunctor<TCLASS>( that, Callback );		
	//////	mNetworkListerners.InsertBack( newFunctor );
	//////}

	//////float GetX()
	//////{
	//////	return mX;
	//////}

	//////float GetY()
	//////{
	//////	return mY;
	//////}

	//////NetworkType GetNetworkType()
	//////{
	//////	return mNetworkType;
	//////}
	
private:

	NetworkServer* mNetworkServer;

	//////List<NetworkFunctor*> mNetworkListerners;

	//////float mX;
	//////float mY;
	//////int mPort;
	//////char mIPAddress[32];
	//////NetworkType mNetworkType;
	//////
	//////int mSocket;	
	//////int mClient;	

};

#endif