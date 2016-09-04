#if defined PLAT_PC

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/network/networksocket.hpp"
#include "common/engine/network/packet.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"

#if defined PLAT_PS3
#include "ps3/ps3defines.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#endif

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

#ifndef PLAT_IOS

NetworkSocket::NetworkSocket()
:	mSocket( INVALID_SOCKET )
,	mID( -1 )
{
}

//===========================================================================

NetworkSocket::NetworkSocket( SOCKET newSocket )
{
#if !defined PLAT_PS3
	mID = -1;	
	mSocket = newSocket;
	setsockopt( mSocket, SOL_SOCKET, SO_DONTLINGER, NULL, 0 );
#endif
}

//===========================================================================

NetworkSocket::~NetworkSocket()
{
}

//===========================================================================

bool NetworkSocket::Connect( unsigned int ip, unsigned int port )
{
#if !defined PLAT_PS3
	struct sockaddr_in socketAddressIn;

	//
	// Create the socket handle
	//
	mSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if (  mSocket == INVALID_SOCKET )
	{
		return false;
	}

	//
	// Set socket options - in this case turn off Nagle algorithm if desired
	//
	int optValue = 1;
	setsockopt( mSocket, IPPROTO_TCP, TCP_NODELAY, (char *)&optValue, sizeof( optValue ) );

	//
	// Last step - set the IP address and port of the server, and call connect()
	//
	socketAddressIn.sin_family = AF_INET;
	socketAddressIn.sin_addr.s_addr = htonl( ip );
	socketAddressIn.sin_port = htons( ( unsigned short )port );

	int returnValue = connect( mSocket, (struct sockaddr *)&socketAddressIn, sizeof( socketAddressIn ) );
	if ( returnValue )
	{
		closesocket( mSocket );
		mSocket = INVALID_SOCKET;
		return false;
	}
#endif
	return true;
}

//===========================================================================

void NetworkSocket::SetBlocking( int block )
{
#if !defined PLAT_PS3
	unsigned long val = block ? 0 : 1;
	ioctlsocket( mSocket, FIONBIO, &val );
#endif
}

//===========================================================================

void NetworkSocket::Send( Packet* packet )
{
	int size = static_cast<int>( mOutBoundPacketList.size() );
	if ( size < 200 )
	{
		mOutBoundPacketList.push_back( packet );
	}
	else
	{
		System::PrintToConsole( "WARNING: Out bound packet buffer list full.\n" );
	}
}

//===========================================================================

bool NetworkSocket::HasOutput()
{
	return !( mOutBoundPacketList.empty() );
}

//===========================================================================

void NetworkSocket::HandleException()
{

}

//===========================================================================

void NetworkSocket::HandleOutput()
{
	//int sendBufferOffset = 0;

	bool dataSent = false;
	do
	{
		Assert( !mOutBoundPacketList.empty(), "" );
		PacketList::iterator iter = mOutBoundPacketList.begin();
		Packet* packet = *iter;

		const char* buffer = packet->GetPacketData();
		int length = packet->GetPacketSize();

		Assert( length < MAX_PACKET_SIZE, "" );

		//int bytesSent = send( mSocket, buffer + sendBufferOffset, length - sendBufferOffset, 0 );
		int bytesSent = send( mSocket, buffer, length, 0 );

		if ( bytesSent > 0 )
		{
			//g_pSocketManager->m_Outbound += rc;
		//	sendBufferOffset += bytesSent;
			dataSent = true;
			Assert( bytesSent == length, "" );
			DELETE_PTR( packet );
			mOutBoundPacketList.pop_front();
		}
		else// if ( WSAGetLastError() != WSAEWOULDBLOCK )
		{
		//	HandleException();
			dataSent = false;
		}
		//else
		//{
		//	fSent = 0;
		//}

		//if ( sendBufferOffset == packet->GetDataSize() )
		//{			
		//	sendBufferOffset = 0;
		//}

	//} while ( fSent && !mOutBoundPacketList.empty() );
	} 
	while ( dataSent && !mOutBoundPacketList.empty() );
}

//===========================================================================

void NetworkSocket::HandleInput()
{
#if !defined PLAT_PS3
	//bool bPktReceived = false;
	//u_long packetSize = 0;
	//int bytesReceived = recv( mSocket, m_recvBuf + m_recvBegin + m_recvOfs, RECV_BUFFER_SIZE - m_recvOfs, 0 );
	int bytesReceived = recv( mSocket, mReceiveBuffer, RECV_BUFFER_SIZE, 0 );
	if ( bytesReceived == 0 )
	{
		return;
	}

	if ( bytesReceived == SOCKET_ERROR  )
	{
		int errorCode = WSAGetLastError();
		printf( "errorCode: %i\n", errorCode );
	}

	//if (rc < 0)
	//{
	//	m_deleteFlag = 1;
	//	return;
	//}

	//const int hdrSize = sizeof(u_long);
	//unsigned int newData = m_recvOfs + rc;
	//int processedData = 0;

	char* receivedBuffer = mReceiveBuffer;

	if ( bytesReceived > 0 )
	{
		do 
		{
			int dataSize = *( reinterpret_cast<int*>( receivedBuffer ) );
			dataSize = ntohl( dataSize );
			receivedBuffer += sizeof ( int );

			Packet* newPacket = NEW_PTR( "Packet" ) Packet( receivedBuffer, dataSize );
			mInBoundPacketList.push_back( newPacket );

			bytesReceived -= dataSize;		
			receivedBuffer += dataSize;		
		} 
		while ( bytesReceived >= MAX_PACKET_SIZE );
	}

	//while (newData > hdrSize)
	//{
	//	// There are two types of packets at the lowest level of our design:
	//	// BinaryPacket - Sends the size as a positive 4 byte integer
	//	// TextPacket - Sends 0 for the size, the parser will search for a CR

	//	packetSize = *(reinterpret_cast<u_long*>(m_recvBuf+m_recvBegin));
	//	packetSize = ntohl(packetSize);
	//	if (m_bBinaryProtocol)
	//	{
	//		// we don't have enough new data to grab the next packet
	//		if (newData < packetSize)
	//			break;

	//		if (packetSize > MAX_PACKET_SIZE)
	//		{
	//			// prevent nasty buffer overruns!
	//			HandleException();
	//			return;
	//		}

	//		if (newData >= packetSize)
	//		{
	//			// we know how big the packet is...and we have the whole thing
	//			shared_ptr<BinaryPacket> pkt(
	//				GCC_NEW BinaryPacket(
	//				&m_recvBuf[m_recvBegin+hdrSize], packetSize-hdrSize));
	//			m_InList.push_back(pkt);
	//			bPktRecieved = true;
	//			processedData += packetSize;
	//			newData -= packetSize;
	//			m_recvBegin += packetSize;
	//		}
	//	}
	//}

	//g_pSocketManager->m_Inbound += rc;
	//m_recvOfs = newData;

	//if (bPktRecieved)
	//{
	//	if (m_recvOfs == 0)
	//	{
	//		m_recvOfs = 0;
	//		m_recvBegin = 0;
	//	}
	//	else if (m_recvBegin + m_recvOfs + MAX_PACKET_SIZE > RECV_BUFFER_SIZE)
	//	{
	//		// we don't want to overrun the buffer - so we copy the leftover bits
	//		// to the beginning of the receive buffer and start over
	//		int leftover = m_recvOfs;
	//		memcpy(m_recvBuf, &m_recvBuf[m_recvBegin], m_recvOfs);
	//		m_recvBegin = 0;
	//	}
	//}
#endif
}

#endif

#endif