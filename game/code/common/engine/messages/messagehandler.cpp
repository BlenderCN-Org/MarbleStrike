//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "messagehandler.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

MessageHandler::MessageHandler()
{	
}

//===========================================================================

MessageHandler::~MessageHandler()
{
	for ( int i = 0; i < PORT_MAX; ++i )
	{
		List<MessageHandlerStruct*>::Iterator iter = mPorts[i].CreateIterator();
		while( iter.HasNext() )
		{
			MessageHandlerStruct* data = iter.Next();
			if ( data->mFunctor != NULL )
			{
				DELETE_PTR( data->mFunctor );
			}
			DELETE_PTR( data );
		}

		mPorts[i].Clear();
	}

	while ( !mMessageQueue.empty() )
	{
		MessageQueueStruct msgStruct = mMessageQueue.front();
		mMessageQueue.pop();
		DELETE_PTR( msgStruct.mMessage );
	}
}

//===========================================================================

void MessageHandler::DispatchMsg()
{
	while ( !mMessageQueue.empty() )
	{
		MessageQueueStruct msgStruct = mMessageQueue.front();
		mMessageQueue.pop();
		
		List<MessageHandlerStruct*>::Iterator iter = mPorts[msgStruct.mPort].CreateIterator();
		while( iter.HasNext() )
		{
			MessageHandlerStruct* data = iter.Next();
			data->mFunctor->Call( msgStruct.mMessage );			
		}

		DELETE_PTR( msgStruct.mMessage );
	}

	for ( int i = 0; i < PORT_MAX; ++i )
	{
		List<MessageHandlerStruct*>::Iterator iter = mPorts[i].CreateIterator();	
		while( iter.HasNext() )
		{
			MessageHandlerStruct* data = iter.Next();
			if ( data->mIsRegistered == false )
			{
				DELETE_PTR( data->mFunctor );
				DELETE_PTR( data );
				iter.Remove();
			}
		}
	}
}

//===========================================================================

void MessageHandler::SendMsg( int port, Message* message )
{
	MessageQueueStruct newMessageItem;
	newMessageItem.mMessage = message;
	newMessageItem.mPort = port;
	mMessageQueue.push( newMessageItem );
}