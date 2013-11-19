#pragma once
//	Packet format from client:
//		int : Packet ID number
//		ServerPacketType : packet type
//	JOIN
//
//	START
//
//	UPDATE
//		int : teamNumber/index number
//		int : number of keys pressed
//		...pressed keys( MonkyKey enum )
//	ACK
//		int : teamNumber/index number
//	QUIT
//		int : teamNumber
//----------------------------------------------------------------------
//	Packet format to client:
//		int : Packet ID number
//		ClientPacketType : packet type
//	REPLY_TO_JOIN	
//		int : teamNumber( if -1 then the game is full )
//	UPDATE
//		for each( in order of player num )
//			vec2f : pos( if -1.f then ship is dead )
//			vec2f : velocity
//				**not used**short : Bit flags for the impulse and rotations to apply
//			int : namelen( namelen > 0 means new ship to spawn )
//			char* : name
//		int : numBulletsToSpawn
//		for each			
//			int : teamNumber
//			float : rotation
//			vec2f : pos
//	ACK
//	
//	DISCONNECTED
//
//	GAME_OVER	
//



#define SERVER_PORT 27015
#define CLIENT_PORT 0
#define PACKET_SIZE 256
#define MAX_NUM_PLAYERS 4
#define MAX_NAME_SIZE 32
// all times are in milliseconds
#define CLIENT_WAIT_TIME_PER_FRAME 1
#define SERVER_WAIT_TIME_PER_FRAME 1
#define WAIT_TIME_BEFORE_SERVER_DROPS_CLIENT 500
#define WAIT_TIME_BEFORE_RESENDING_PACKET 500

#define APPLY_FORWARD_IMPULSE 0x0001
#define APPLY_REVERSE_IMPULSE 0x0002
#define APPLY_CLOCKWISE_ROTATION 0x0040
#define APPLY_COUNTER_CLOCKWISE_ROTATION 0x0080

namespace Monky
{
	enum ClientPacketType
	{
		CLIENT_PACKET_REPLY_TO_JOIN,
		CLIENT_PACKET_UPDATE,
		CLIENT_PACKET_DISCONNECTED,
		CLIENT_PACKET_GAME_OVER,
	};

	enum ServerPacketType
	{
		SERVER_PACKET_JOIN,
		SERVER_PACKET_START,
		SERVER_PACKET_UPDATE,
		SERVER_PACKET_ACK,
		SERVER_PACKET_QUIT,
		SERVER_PACKET_INVALID
	};
}