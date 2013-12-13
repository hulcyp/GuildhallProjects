#include "Button.h"
#include "MonkyRenderer/Mesh.h"

namespace Monky
{
	Mesh* GenerateButtonMeshFromAABB2( const aabb2f& box, const std::string& material );

	Button::Button( const aabb2f& boundingBox, const std::string& upMaterial )
		:	m_boundingBox( boundingBox )
		,	m_upMaterial( upMaterial )
		,	m_downMaterial( upMaterial )
	{
		m_mesh = GenerateButtonMeshFromAABB2( boundingBox, upMaterial );
	}

	void Button::SetDownMaterial( const std::string& downMaterial )
	{
		m_downMaterial = downMaterial;
	}

	void Button::SetEventToFire( const std::string& eventName )
	{
		m_eventToFire = eventName;
	}

	void Button::SetEventToFireWithParams( const std::string& eventName, NamedProperties& params )
	{
		m_eventToFire = eventName;
		m_params = params;
	}

	void Button::FingerDown( const vec2f& fingerPos )
	{
		if( m_boundingBox.contains( fingerPos ) )
		{
			fireEvent( m_eventToFire, m_params );
			//consolePrintf( "Button Pressed" );
			m_mesh->setMaterial( m_downMaterial );
		}
	}

	void Button::FingerUp( const vec2f& fingerPos )
	{
		m_mesh->setMaterial( m_upMaterial );
	}

	void Button::Render()
	{
		NamedProperties params;
		matStackf modelStack;
		modelStack.loadIdentity();
		modelStack.translate( m_boundingBox.getCenter() );
		params.set( "modelMatrix", modelStack.top() );
		params.set( "mesh", m_mesh );
		fireEvent( "renderMesh", params );
		//consolePrintf( "Button box: %s", m_boundingBox.toString().c_str() );
	}

	void Button::ReloadMesh()
	{
		m_mesh->reloadBuffers();
	}

	Mesh* GenerateButtonMeshFromAABB2( const aabb2f& box, const std::string& material )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		Color4f color = color::WHITE;

		float hWidth = box.getWidth() * 0.5f;
		float hHeight = box.getHeight() * 0.5f;

		vertices.push_back( Mesh::Vertex( vec3f( -hWidth, -hHeight ),
									vec3f(),
									color,
									vec2f( 0.0f, 0.0f ) ) );


		vertices.push_back( Mesh::Vertex( vec3f( hWidth, -hHeight ),
									vec3f(),
									color,
									vec2f( 1.0f, 0.0f ) ) );

		vertices.push_back( Mesh::Vertex( vec3f( hWidth, hHeight ),
									vec3f(),
									color,
									vec2f( 1.0f, 1.0f ) ) );


		vertices.push_back( Mesh::Vertex( vec3f( -hWidth, hHeight ),
									vec3f( 0.0f, 0.0f, 1.0f ),
									color,
									vec2f( 0.0f, 1.0f ) ) );


		indices.push_back( 0 );
		indices.push_back( 2 );
		indices.push_back( 3 );

		indices.push_back( 2 );
		indices.push_back( 0 );
		indices.push_back( 1 );

		return new Mesh( vertices, indices, material );
	}
}
