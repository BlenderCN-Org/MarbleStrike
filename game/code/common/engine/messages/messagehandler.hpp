#ifndef MESSAGEHANDLER_HPP
#define MESSAGEHANDLER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/list.hpp"
#include "common/engine/messages/message.hpp"
#include "common/engine/system/classcallback.hpp"
#include <queue>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class Message;

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

class TFunctor;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

enum MessagePortType
{
	PORT_GAME,	
	PORT_MAX
};

class MessageHandlerStruct
{
public:
	MessageHandlerStruct() 
		:	mFunctor( NULL ) 
		,	mIsRegistered( false )
	{
	}

	bool mIsRegistered;
	ClassCallback* mFunctor;
};

struct MessageQueueStruct
{
	Message* mMessage;
	int mPort;
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class MessageHandler
{
public:

	MessageHandler();
	~MessageHandler();

	void DispatchMsg();
	void SendMsg( int port, Message* message );	

	template<typename TCLASS>
	void RegisterListener( int port, void ( TCLASS::*Callback )( void* ), TCLASS* that )
	{	
		ClassCallback* functor = NEW_PTR( "Functor" ) ClassCallbackSpecific<TCLASS>( that, Callback );
		MessageHandlerStruct* data = NEW_PTR( "Message Struct" ) MessageHandlerStruct;
		data->mFunctor = functor;
		data->mIsRegistered = true;
		mPorts[port].InsertBack( data );
	}


	template<typename TCLASS>
	void UnRegisterListener( int port, void ( TCLASS::*Callback )( void* ), TCLASS* that )
	{	
		List<MessageHandlerStruct*>::Iterator iter = mPorts[port].CreateIterator();
		while( iter.HasNext() )
		{
			MessageHandlerStruct* data = iter.Next();
			ClassCallbackSpecific<TCLASS>* functor = (ClassCallbackSpecific<TCLASS>*)data->mFunctor;
			if (functor->GetObjectThis() == that && functor->GetCallback() == Callback )
			{
				data->mIsRegistered = false;

				return;
			}
		}

		Assert( false, "" );
	}

private:

	List<MessageHandlerStruct*> mPorts[PORT_MAX];	
	std::queue<MessageQueueStruct> mMessageQueue;
};

#endif
