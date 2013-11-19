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
//			-vec2f : pos( if -1.f then ship is dead )
//			-vec2f : velocity
//			int : namelen( namelen > 0 means new ship to spawn else ship is dead )
//			char* : name
//			Timeline update
//				int : lengthOfName
//				char* : name 
//				int : numPairsToupdate
//				for each
//					std::pair< time_t, SGA_State > : pair 
//
//		Bullet update
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
//----------------------------------------------------------------------
//	Timeline Packet:
//		TIMELINE_UPDATE
//		lengthOfName : int
//		name : char*
//		numPairsToupdate
//		for each
//			pair : std::pair< time_t, SGA_State >
//
//
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

#define INPUT_DELAY_TIME 50


namespace Monky
{

	enum PacketType
	{
		CLIENT_PACKET_REPLY_TO_JOIN,
		CLIENT_PACKET_UPDATE,
		CLIENT_PACKET_DISCONNECTED,
		CLIENT_PACKET_GAME_OVER,
		SERVER_PACKET_JOIN,
		SERVER_PACKET_START,
		SERVER_PACKET_UPDATE,
		SERVER_PACKET_ACK,
		SERVER_PACKET_QUIT,
		SERVER_PACKET_INVALID,
		TIMELINE_UPDATE
	};
}