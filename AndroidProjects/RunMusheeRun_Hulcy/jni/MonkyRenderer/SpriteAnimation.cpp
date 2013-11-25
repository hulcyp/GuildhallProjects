#include "SpriteAnimation.h"
#include "CoreEngine/CommonCoreEngineIncludes.h"
#include "Texture.h"
#include "CoreEngine/TimeUtils.h"
#include "CoreEngine/XMLParser.h"

namespace Monky
{
	SpriteAnimation::Frame::Frame()
		:	material( "" )
	{}
	//---------------------------------------------------------------------------------
	SpriteAnimation::Frame::Frame( const Frame& copy )
		:	material( copy.material )
	{
		texCoords[0] = copy.texCoords[0];
		texCoords[1] = copy.texCoords[1];
	}
	//---------------------------------------------------------------------------------
	SpriteAnimation::SpriteAnimation()
		:	m_frameRate( 0.0f )
		,	m_isLoop( false )
		,	m_material( "" )
		, 	m_isPlaying( false )
		,	m_lastFrameTime( 0.0f )
		,	m_currentFrameNumber( 0 )
	{}
	//---------------------------------------------------------------------------------
	SpriteAnimation::SpriteAnimation( const std::string& animationFile )
		:	m_frameRate( 0.0f )
		,	m_isLoop( false )
		,	m_material( "" )
		, 	m_isPlaying( false )
		,	m_lastFrameTime( 0.0f )
		,	m_currentFrameNumber( 0 )
	{
		XMLParser parser( animationFile.c_str(), false );
		XMLDocument& doc = parser.getDocument();

		if( !doc.Error() )
		{
			consolePrintf( "Loading animation: %s", animationFile.c_str() );
			XMLNode* animation = doc.FirstChildElement( "Animation" );
			parser.validateXMLAttributes( animation, "frameRate,material", "isLooping,startPlaying" );
			parser.validateXMLChildElements( animation, "Frame", "" );

			float frameRate = parser.getXMLAttributeAsFloat( animation, "frameRate", 0.0f );
			bool isLooping = parser.getXMLAttributeAsBool( animation, "isLooping", false );
			bool startPlaying = parser.getXMLAttributeAsBool( animation, "startPlaying", false );
			std::string material = parser.getXMLAttributeAsString( animation, "material", "" );

			m_frameRate = frameRate;
			m_isLoop = isLooping;
			m_material = material;

			XMLNode* frame;
			for( frame = animation->FirstChildElement( "Frame" ); frame != nullptr; frame = frame->NextSiblingElement( "Frame" ) )
			{
				parser.validateXMLChildElements( frame, "TexCoords", "Material" );
				XMLNode* texCoords = frame->FirstChildElement( "TexCoords" );
				XMLNode* material = frame->FirstChildElement( "Material" );
				parser.validateXMLAttributes( texCoords, "topLeft,width,height", "" );
				vec2f topLeft = parser.getXMLAttributeAsVec2( texCoords, "topLeft", vec2f() );
				float width = parser.getXMLAttributeAsFloat( texCoords, "width", 1.0f );
				float height = parser.getXMLAttributeAsFloat( texCoords, "height", 1.0f );

				std::string materialName;
				if( material != nullptr )
				{
					parser.validateXMLAttributes( material, "name", "" );
					 materialName = parser.getXMLAttributeAsString( material, "name", "" );

				}
				AddFrame( topLeft, width, height, materialName );
			}

			if( startPlaying )
			{
				Play();
			}
			consolePrintf( "Successfully loaded animation: %s", animationFile.c_str() );
		}
	}
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
		SpriteAnimation::Frame frame;
		if( material.size() != 0 )
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
			//consolePrintf( "Current frame: %d", m_currentFrameNumber );
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
				m_currentFrameNumber = m_frames.size() - 1;
				Pause();
			}
		}
	}
}
