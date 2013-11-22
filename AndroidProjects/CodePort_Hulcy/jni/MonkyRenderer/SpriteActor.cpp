#include "SpriteActor.h"
#include "SpriteAnimation.h"
#include "VtxBufferAccessor.h"
#include "Mesh.h"
#include "MeshFactory.h"
#include "GLBuffer.h"

namespace Monky
{
	SpriteActor::SpriteActor( const std::string& name, float radius, const std::string& material, float drag, float mass )
		:	Actor( name, MeshFactory::generate2DOrthoRectangle( radius * 0.5f, radius * 0.5f, material ) )
		,	m_radius( radius )
		,	m_drag( drag )
		,	m_mass( mass )
	{
		Init();
	}
	//---------------------------------------------------------------------------------
	SpriteActor::~SpriteActor()
	{
		MAP_CLEANUP( m_animations );
	}
	//---------------------------------------------------------------------------------
	void SpriteActor::ApplyImpulse( const vec2f& impulse )
	{
		m_vel += impulse / m_mass;
	}
	//---------------------------------------------------------------------------------
	void SpriteActor::ApplyGravity( const vec2f& gravity )
	{
		m_vel += gravity;
	}
	//---------------------------------------------------------------------------------
	void SpriteActor::update( double deltaTime )
	{
		Actor::update( deltaTime );
		float dt = (float)deltaTime;
		vec3f pos = getWorldPosition();
		vec2f pos2D = pos.xy();

		m_vel -= m_vel * dt * m_drag;
		pos2D += m_vel * dt;
		setPosition( pos2D );

		if( m_currentAnimation != nullptr )
			m_currentAnimation->Update( dt );
	}
	//---------------------------------------------------------------------------------
	bool SpriteActor::OnCollision( SpriteActor* other )
	{
		return true;
	}
	//---------------------------------------------------------------------------------
	void SpriteActor::SetVisible( bool visible )
	{
		m_visible = visible;
	}
	//---------------------------------------------------------------------------------
	void SpriteActor::render()
	{
		if( m_visible )
		{
			Mesh* mesh = getMesh();
			Mesh::TriangleBatch* triBatch = mesh->getTriangleBatch(0);
			const SpriteAnimation::Frame& frame = m_currentAnimation->GetCurrentFrame();

			std::memcpy( triBatch->vboAccessor->getUV(0), &frame.texCoords[SpriteAnimation::Frame::TOP_LEFT].x, sizeof( vec2f ) );
			vec2f topRight = frame.texCoords[SpriteAnimation::Frame::TOP_LEFT] + vec2f( frame.texCoords[SpriteAnimation::Frame::BOTTOM_RIGHT].x, 0.0f );
			std::memcpy( triBatch->vboAccessor->getUV(1),  &topRight.x, sizeof( vec2f ) );
			std::memcpy( triBatch->vboAccessor->getUV(2), &frame.texCoords[SpriteAnimation::Frame::BOTTOM_RIGHT].x, sizeof( vec2f ) );
			vec2f botLeft = frame.texCoords[SpriteAnimation::Frame::TOP_LEFT] + vec2f( 0.0f, frame.texCoords[SpriteAnimation::Frame::BOTTOM_RIGHT].y );
			std::memcpy( triBatch->vboAccessor->getUV(3), &botLeft.x, sizeof( vec2f ) );

			triBatch->vboBuffer->sendToOpenGL();
			mesh->setMaterial( frame.material, 0 );

			Actor::render();
		}
	}
	//---------------------------------------------------------------------------------
	void SpriteActor::AddAnimation( const std::string& animationName, SpriteAnimation* animation )
	{
		SpriteAnimation*& anim  = m_animations[ animationName ];
		SAFE_DELETE( anim )
		anim = animation;
	}
	//---------------------------------------------------------------------------------
	void SpriteActor::PlayAnimation( const std::string& animationName )
	{
		std::map< std::string, SpriteAnimation* >::iterator iter = m_animations.find( animationName );
		if( iter != m_animations.end() )
		{
			iter->second->Play();
			m_currentAnimationName = animationName;
			m_currentAnimation = iter->second;
		}
	}
	//---------------------------------------------------------------------------------
	const std::string& SpriteActor::GetCurrentAnimationName()
	{
		return m_currentAnimationName;
	}
	//---------------------------------------------------------------------------------
	bool SpriteActor::HasAnimation( const std::string& animationName )
	{
		return ( m_animations.find( animationName ) != m_animations.end() );
	}
	//---------------------------------------------------------------------------------
	void SpriteActor::Init()
	{
		SpriteAnimation* anim = new SpriteAnimation();
		anim->AddFrame( vec2f(), 1.0f, 1.0f, getMesh()->getMaterial(0) );
		m_animations[ "default" ] = anim;
	}


}
