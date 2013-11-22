#include "SpriteAnimation.h"
#include "CoreEngine/CommonCoreEngineIncludes.h"
#include "Texture.h"
#include "CoreEngine/TimeUtils.h"

namespace Monky
{
	SpriteAnimation::SpriteAnimation()
		:	m_frameRate( 0.0f )
		,	m_isLoop( false )
		,	m_material( "" )
		, 	m_isPlaying( false )
		,	m_lastFrameTime( 0.0f )
	{}
	//---------------------------------------------------------------------------------
	SpriteAnimation::~SpriteAnimation()
	{
		m_frames.clear();
	}
	//---------------------------------------------------------------------------------
	void SpriteAnimation::SetMaterial( const std::string& material )
	{
		m_material = material;
	}
	//---------------------------------------------------------------------------------
	void SpriteAnimation::SetFrameDuration( float duration )
	{
		m_frameRate = duration;
	}
	//---------------------------------------------------------------------------------
	void SpriteAnimation::AddFrame( const vec2f& topLeft, float width, float height, const std::string& material )
	{
		Frame frame;
		if( material.size() == 0 )
			frame.material = material;
		else
			frame.material = m_material;
		frame.texCoords[Frame::TOP_LEFT] = topLeft;
		frame.texCoords[Frame::BOTTOM_RIGHT] = topLeft + vec2f( width, height );

		m_frames.push_back( frame );
	}
	//---------------------------------------------------------------------------------
	void SpriteAnimation::SetIsLoop( bool isLooping )
	{
		m_isLoop = isLooping;
	}
	//---------------------------------------------------------------------------------
	void SpriteAnimation::Update( float deltaTime )
	{
		if( m_frameRate != 0 && m_isPlaying )
		{
			float absoluteTime = TimeUtils::GetCurrentApplicationTimeSeconds();
			float timeToNextFrame = m_lastFrameTime + m_frameRate;

			if( absoluteTime > timeToNextFrame )
			{
				Advance();
				m_lastFrameTime = absoluteTime;
			}
		}
	}
	//---------------------------------------------------------------------------------
	void SpriteAnimation::Play()
	{
		m_isPlaying = true;
		m_lastFrameTime = TimeUtils::GetCurrentApplicationTimeSeconds();
	}
	//---------------------------------------------------------------------------------
	void SpriteAnimation::Pause()
	{
		m_isPlaying = false;
	}
	//---------------------------------------------------------------------------------
	void SpriteAnimation::Stop()
	{
		m_isPlaying = false;
		m_currentFrameNumber = 0;
	}
	//---------------------------------------------------------------------------------
	const SpriteAnimation::Frame& SpriteAnimation::GetCurrentFrame() const
	{
		return m_frames[ m_currentFrameNumber ];
	}
	//---------------------------------------------------------------------------------
	const SpriteAnimation::Frame& SpriteAnimation::GetFrame( int frameIndex ) const
	{
		assertion( frameIndex >= 0 && frameIndex < m_frames.size(), "Frame number out of bounds" );
		return m_frames[ frameIndex ];
	}
	//---------------------------------------------------------------------------------
	//--Protected members
	//---------------------------------------------------------------------------------
	void SpriteAnimation::Advance()
	{
		++m_currentFrameNumber;
		if( m_currentFrameNumber >= m_frames.size() )
		{
			if( m_isLoop )
			{
				m_currentFrameNumber = 0;
			}
			else
			{
				--m_currentFrameNumber;
				Pause();
			}
		}
	}
}
