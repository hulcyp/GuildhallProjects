#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 9/30/2013
//	Description:
//
//---------------------------------------------
#include "Timeline.h"

namespace Monky
{
	struct SGA_State
	{
		vec2f pos;
		vec2f vel;
		vec2f dir;
		float rot;
		bool spawnBullet;

		SGA_State();

		SGA_State operator*( float value ) const;
		const SGA_State& operator=( const SGA_State& copy );
		SGA_State operator+( const SGA_State& rhs ) const;
	};

	class SGA_StateTimeline : public Timeline< SGA_State >
	{
	public:
		SGA_StateTimeline( const std::string& name, float drag = 0.0f );
		~SGA_StateTimeline();

		virtual void SendUpdate( int packetID );
		virtual void RecieveUpdate( Packet* packet, bool apply = true );

		void SetDrag( float drag ) { m_drag = drag; }
		float GetDrag() const { return m_drag; }

		void AddUpdateToPacket( Packet* packet );

	protected:
		virtual SGA_State Interpolate( const std::pair< time_t, SGA_State >& first, const std::pair< time_t, SGA_State >& second, time_t sampleAt ) const;
		virtual SGA_State Extrapolate( const std::pair< time_t, SGA_State >& first, const std::pair< time_t, SGA_State >& second, time_t sampleAt ) const;

	private:
		Packet* m_packet;
		float m_drag;
		

	};
}