#pragma once
#include "CoreEngine/CommonCoreEngineIncludes.h"
#include <vector>
#include <string>

namespace Monky
{

	class SpriteAnimation
	{
	public:
		
		struct Frame
		{
			std::string material;
			vec2f texCoords[2];

			static const int TOP_LEFT = 0;
			static const int BOTTOM_RIGHT = 1;
		};

		SpriteAnimation();
		~SpriteAnimation();

		void SetMaterial( const std::string& materialName );
		void SetFrameDuration( float duration );
		void AddFrame( const vec2f& topLeft, float width, float height, const std::string& material = "" );
		void SetIsLoop( bool isLooping );
		void Update( float deltaTime );

		void Play();
		void Pause();
		void Stop();

		const Frame& GetCurrentFrame() const;
		const Frame& GetFrame( int frameIndex ) const;
		float GetFrameRate() const { return m_frameRate; }

	protected:
		void Advance();


	private:
		std::vector< Frame > m_frames;
		std::string m_material;
		float m_frameRate;
		int m_currentFrameNumber;
		bool m_isLoop;
		bool m_isPlaying;
		float m_lastFrameTime;

	};
}
