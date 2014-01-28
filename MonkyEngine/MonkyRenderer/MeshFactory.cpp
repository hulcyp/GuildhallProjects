#include "MeshFactory.h"
#include "Mesh.h"
#include "GLBuffer.h"
#include "BufferLayout.h"
#include "VtxBufferAccessor.h"

namespace Monky
{
	Mesh* MeshFactory::generateCube( float size, const std::string& materialName, Color4f color )
	{
		float hSize = size;
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;
		
		// +X axis face
		vertices.push_back( Mesh::Vertex(	vec3f( size, size, size ),
											vec3f( 1.0f, 0.0f, 0.0f ),
											color,
											vec2f( 0.0f, 0.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( size, size, 0.0f ),
											vec3f( 1.0f, 0.0f, 0.0f ),
											color,
											vec2f( 1.0f, 0.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( size, 0.0f, 0.0f ),
											vec3f( 1.0f, 0.0f, 0.0f ),
											color,
											vec2f( 1.0f, 1.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( size, 0.0f, size ),
											vec3f( 1.0f, 0.0f, 0.0f ),
											color,
											vec2f( 0.0f, 1.0f ) ) );
		
		indices.push_back( 0 );
		indices.push_back( 2 );
		indices.push_back( 3 );
		indices.push_back( 2 );
		indices.push_back( 0 );
		indices.push_back( 1 );

		// -X axis face
		vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, size, 0.0f ),
											vec3f( -1.0f, 0.0f, 0.0f ),
											color,
											vec2f( 0.0, 0.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, size, size ),
											vec3f( -1.0f, 0.0f, 0.0f ),
											color,
											vec2f( 1.0f, 0.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, 0.0f, size ),
											vec3f( -1.0f, 0.0f, 0.0f ),
											color,
											vec2f( 1.0f, 1.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, 0.0f, 0.0f ),
											vec3f( -1.0f, 0.0f, 0.0f ),
											color,
											vec2f( 0.0f, 1.0f ) ) );
		indices.push_back( 4 );
		indices.push_back( 6 );
		indices.push_back( 7 );
		indices.push_back( 6 );
		indices.push_back( 4 );
		indices.push_back( 5 );
				
		// +Z axis face
		vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, size, size ),
											vec3f( 0.0f, 0.0f, 1.0f ),
											color,
											vec2f( 0.0f, 0.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( size, size, size ),
											vec3f( 0.0f, 0.0f, 1.0f ),
											color,
											vec2f( 1.0f, 0.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( size, 0.0f, size ),
											vec3f( 0.0f, 0.0f, 1.0f ),
											color,
											vec2f( 1.0f, 1.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, 0.0f, size ),
											vec3f( 0.0f, 0.0f, 1.0f ),
											color,
											vec2f( 0.0f, 1.0f ) ) );
		indices.push_back( 8 );
		indices.push_back( 10 );
		indices.push_back( 11 );
		indices.push_back( 10 );
		indices.push_back( 8 );
		indices.push_back( 9 );

		// -Z axis face
		vertices.push_back( Mesh::Vertex(	vec3f( size, size, 0.0f ),
											vec3f( 0.0f, 0.0f, -1.0f ),
											color,
											vec2f( 0.0f, 0.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, size, 0.0f ),
											vec3f( 0.0f, 0.0f, -1.0f ),
											color,
											vec2f( 1.0f, 0.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, 0.0f, 0.0f ),
											vec3f( 0.0f, 0.0f, -1.0f ),
											color,
											vec2f( 1.0f, 1.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( size, 0.0f, 0.0f ),
											vec3f( 0.0f, 0.0f, -1.0f ),
											color,
											vec2f( 0.0f, 1.0f ) ) );
		indices.push_back( 12 );
		indices.push_back( 14 );
		indices.push_back( 15 );
		indices.push_back( 14 );
		indices.push_back( 12 );
		indices.push_back( 13 );

		// +Y axis face
		vertices.push_back( Mesh::Vertex(	vec3f( size, size, 0.0f ),
											vec3f( 0.0f, 1.0f, 0.0f ),
											color,
											vec2f( 0.0f, 0.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( size, size, size ),
											vec3f( 0.0f, 1.0f, 0.0f ),
											color,
											vec2f( 1.0f, 0.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, size, size ),
											vec3f( 0.0f, 1.0f, 0.0f ),
											color,
											vec2f( 1.0f, 1.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, size, 0.0f ),
											vec3f( 0.0f, 1.0f, 0.0f ),
											color,
											vec2f( 0.0f, 1.0f ) ) );
		indices.push_back( 16 );
		indices.push_back( 18 );
		indices.push_back( 19 );
		indices.push_back( 18 );
		indices.push_back( 16 );
		indices.push_back( 17 );

		// -Y axis face
		vertices.push_back( Mesh::Vertex(	vec3f( size, 0.0f, size ),
											vec3f( 0.0f, -1.0f, 0.0f ),
											color,
											vec2f( 0.0f, 0.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( size, 0.0f, 0.0f ),
											vec3f( 0.0f, -1.0f, 0.0f ),
											color,
											vec2f( 1.0f, 0.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, 0.0f, 0.0f ),
											vec3f( 0.0f, -1.0f, 0.0f ),
											color,
											vec2f( 1.0f, 1.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, 0.0f, size ),
											vec3f( 0.0f, -1.0f, 0.0f ),
											color,
											vec2f( 0.0f, 1.0f ) ) );
		indices.push_back( 20 );
		indices.push_back( 22 );
		indices.push_back( 23 );
		indices.push_back( 22 );
		indices.push_back( 20 );
		indices.push_back( 21 );

		for( size_t i = 0; i < vertices.size(); ++i )
		{
			vertices[i].x -= size * .5f;
			vertices[i].y -= size * .5f;
			vertices[i].z -= size * .5f;
		}
		

		Mesh* mesh = new Mesh( vertices, indices, materialName, GL_TRIANGLES );
		return mesh;
	}
	//-------------------------------------------------------------------------------------------------------------------------
	// Algorithm obtianed from: http://stackoverflow.com/questions/7946770/calculating-a-sphere-in-opengl

	Mesh* MeshFactory::generateSphere( float radius, int rings, int sectors, const std::string& materialName, Color4f color )
	 {	
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

        float const R = 1.f/(float)(rings-1);
        float const S = 1.f/(float)(sectors-1);
        int r, s;

        for(r = 0; r < rings; r++) 
		{
			for(s = 0; s < sectors; s++) 
			{
                float const y = sin( -MathFuncs<float>::PI*.5f + MathFuncs<float>::PI * r * R );
                float const x = cos(2*MathFuncs<float>::PI * s * S) * sin( MathFuncs<float>::PI * r * R );
                float const z = sin(2*MathFuncs<float>::PI * s * S) * sin( MathFuncs<float>::PI * r * R );

				vec2f uv( s*S, r*R );
				vec3f verts( x * radius, y * radius, z * radius );
				vec3f norm( x, y, z );
				vertices.push_back( Mesh::Vertex( verts, norm, color, uv ) );
			}
		}

        for(r = 0; r < rings; r++) 
		{
			for(s = 0; s < sectors; s++) 
			{
				indices.push_back( r * sectors + s );
				indices.push_back( r * sectors + (s+1) );
                indices.push_back( (r+1) * sectors + (s+1) );
                indices.push_back( (r+1) * sectors + s );
			}
		}

		Mesh* mesh = new Mesh( vertices, indices, materialName, GL_QUADS );
		return mesh;
    }
	//-------------------------------------------------------------------------------------------------------------------------
	Mesh* MeshFactory::generateXYPlane( float size, const std::string& materialName )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, size, 0.0f ),
									vec3f( 0.0f, 0.0f, 1.0f ),
									color::WHITE,
									vec2f( 0.0f, 0.0f ) ) );
		
		vertices.push_back( Mesh::Vertex( vec3f( size, size, 0.0f ),
									vec3f( 0.0f, 0.0f, 1.0f ),
									color::WHITE,
									vec2f( 1.0f, 0.0f ) ) );
		
		vertices.push_back( Mesh::Vertex( vec3f( size, 0.0f, 0.0f ),
									vec3f( 0.0f, 0.0f, 1.0f ),
									color::WHITE,
									vec2f( 1.0f, 1.0f ) ) );

		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, 0.0f, 0.0f ),
									vec3f( 0.0f, 0.0f, 1.0f ),
									color::WHITE,
									vec2f( 0.0f, 1.0f ) ) );

		indices.push_back( 0 );
		indices.push_back( 2 );
		indices.push_back( 3 );
		
		indices.push_back( 2 );
		indices.push_back( 0 );
		indices.push_back( 1 );

		
		Mesh* mesh = new Mesh( vertices, indices, materialName );
		return mesh;

	}
	//-------------------------------------------------------------------------------------------------------------------------
	Mesh* MeshFactory::generateAxis( float length, const std::string& materialName )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		vertices.push_back( Mesh::Vertex( vec3f(), 
									vec3f(),
									color::RED,
									vec2f() ) );
		vertices.push_back( Mesh::Vertex( vec3f( length, 0.0f, 0.0f ),
									vec3f(),
									color::RED,
									vec2f() ) );

		vertices.push_back( Mesh::Vertex( vec3f(),
									vec3f(),
									color::GREEN,
									vec2f() ) );
		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, length, 0.0f ),
									vec3f(),
									color::GREEN,
									vec2f() ) );

		vertices.push_back( Mesh::Vertex( vec3f(),
									vec3f(),
									color::BLUE,
									vec2f() ) );
		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, 0.0f, length ),
									vec3f(),
									color::BLUE,
									vec2f() ) );
		
		for( size_t i = 0; i < vertices.size(); ++i )
		{
			indices.push_back( i );
		}

		return new Mesh( vertices, indices, materialName, GL_LINES );

	}
	//-------------------------------------------------------------------------------------------------------------------------
	Mesh* MeshFactory::generateAxis( float length, Color4f color, const std::string& materialName )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		vertices.push_back( Mesh::Vertex( vec3f(), 
									vec3f(),
									color,
									vec2f() ) );
		vertices.push_back( Mesh::Vertex( vec3f( length, 0.0f, 0.0f ),
									vec3f(),
									color,
									vec2f() ) );

		vertices.push_back( Mesh::Vertex( vec3f(),
									vec3f(),
									color,
									vec2f() ) );
		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, length, 0.0f ),
									vec3f(),
									color,
									vec2f() ) );

		vertices.push_back( Mesh::Vertex( vec3f(),
									vec3f(),
									color,
									vec2f() ) );
		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, 0.0f, length ),
									vec3f(),
									color,
									vec2f() ) );
		
		for( size_t i = 0; i < vertices.size(); ++i )
		{
			indices.push_back( i );
		}

		return new Mesh( vertices, indices, materialName, GL_LINES );
	}
	//-------------------------------------------------------------------------------------------------------------------------
	Mesh* MeshFactory::generatePointMesh( float radius, const std::string& materialName, Color4f color )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		vertices.push_back( Mesh::Vertex( vec3f( radius, radius, radius ),
									vec3f(),
									color,
									vec2f() ) );

		vertices.push_back( Mesh::Vertex( vec3f( -radius, -radius, -radius ),
									vec3f(),
									color,
									vec2f() ) );

		vertices.push_back( Mesh::Vertex( vec3f( -radius, radius, -radius ),
									vec3f(),
									color,
									vec2f() ) ) ;

		vertices.push_back( Mesh::Vertex( vec3f( radius, -radius, radius ),
									vec3f(),
									color,
									vec2f() ) );

		vertices.push_back( Mesh::Vertex( vec3f( -radius, radius, radius ),
									vec3f(),
									color,
									vec2f() ) );
		
		vertices.push_back( Mesh::Vertex( vec3f( radius, -radius, -radius ),
									vec3f(),
									color,
									vec2f() ) );

		vertices.push_back( Mesh::Vertex( vec3f( radius, -radius, -radius ),
									vec3f(),
									color,
									vec2f() ) );

		vertices.push_back( Mesh::Vertex( vec3f( -radius, radius, radius ),
									vec3f(),
									color,
									vec2f() ) );

		vertices.push_back( Mesh::Vertex( vec3f( radius, radius, -radius ),
									vec3f(),
									color,
									vec2f() ) );

		vertices.push_back( Mesh::Vertex( vec3f( -radius, -radius, radius ),
									vec3f(),
									color,
									vec2f() ) );

		for( size_t i = 0; i < vertices.size(); ++i )
		{
			indices.push_back( i );
		}

		return new Mesh( vertices, indices, materialName, GL_LINES );
	}
	//-------------------------------------------------------------------------------------------------------------------------
	Mesh* MeshFactory::generateArrow( const vec3f& dir, float length, const std::string& materialName, Color4f color )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		//float lengthDivFour = length * 0.25f;
		//float xArrowHeadDist = length - lengthDivFour;

		vertices.push_back( Mesh::Vertex( vec3f(),
									vec3f(),
									color::GREEN,
									vec2f() ) );

		vertices.push_back( Mesh::Vertex(	dir*length,
									vec3f(),
									color::RED,
									vec2f() ) );

		//vertices.push_back( Vertex(	vec3f( xArrowHeadDist, lengthDivFour, -lengthDivFour ),
		//							vec3f(),
		//							color::RED,
		//							vec2f() ) );
		//vertices.push_back( Vertex(	vec3f( xArrowHeadDist, lengthDivFour, lengthDivFour ),
		//							vec3f(),
		//							color::RED,
		//							vec2f() ) );
		//vertices.push_back( Vertex(	vec3f( xArrowHeadDist, -lengthDivFour, -lengthDivFour ),
		//							vec3f(),
		//							color::RED,
		//							vec2f() ) );
		//vertices.push_back( Vertex(	vec3f( xArrowHeadDist, -lengthDivFour, lengthDivFour ),
		//							vec3f(),
		//							color::RED,
		//							vec2f() ) );
		//

		
		indices.push_back( 0 );
		indices.push_back( 1 );
		//indices.push_back( 1 );
		//indices.push_back( 2 );
		//indices.push_back( 1 );
		//indices.push_back( 3 );
		//indices.push_back( 1 );
		//indices.push_back( 4 );
		//indices.push_back( 1 );
		//indices.push_back( 5 );

		return new Mesh( vertices, indices, materialName, GL_LINES );
	}
	//-------------------------------------------------------------------------------------------------------------------------
	Mesh* MeshFactory::generate2DOrthoRectangle( float width, float height, const std::string& materialName, Color4f color )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, height ),
									vec3f(),
									color,
									vec2f( 0.0f, 0.0f ) ) );

		vertices.push_back( Mesh::Vertex( vec3f( width, height ),
									vec3f(),
									color,
									vec2f( 1.0f, 0.0f ) ) );

		vertices.push_back( Mesh::Vertex( vec3f( width, 0.0f ),
									vec3f(),
									color,
									vec2f( 1.0f, 1.0f ) ) );
		
		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, 0.0f ),
									vec3f( 0.0f, 0.0f, 1.0f ),
									color,
									vec2f( 0.0f, 1.0f ) ) );
		
		
		indices.push_back( 0 );
		indices.push_back( 2 );
		indices.push_back( 3 );
		
		indices.push_back( 2 );
		indices.push_back( 0 );
		indices.push_back( 1 );

		return new Mesh( vertices, indices, materialName );
	}
	//-------------------------------------------------------------------------------------------------------------------------
	Mesh* MeshFactory::generate2DOrthoRectangleBorder( float width, float height, const std::string& materialName, Color4f color )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, height ),
									vec3f(),
									color,
									vec2f( 0.0f, 1.0f ) ) );

		vertices.push_back( Mesh::Vertex( vec3f( width, height ),
									vec3f(),
									color,
									vec2f( 1.0f, 1.0f ) ) );

		vertices.push_back( Mesh::Vertex( vec3f( width, 0.0f ),
									vec3f(),
									color,
									vec2f( 1.0f, 0.0f ) ) );
		
		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, 0.0f ),
									vec3f( 0.0f, 0.0f, 1.0f ),
									color,
									vec2f( 0.0f, 0.0f ) ) );
		
		
		indices.push_back( 0 );
		indices.push_back( 1 );
		indices.push_back( 1 );
		indices.push_back( 2 );
		indices.push_back( 2 );
		indices.push_back( 3 );
		indices.push_back( 3 );
		indices.push_back( 0 );

		return new Mesh( vertices, indices, materialName, GL_LINES );
	}
	//-------------------------------------------------------------------------------------------------------------------------
	Mesh* MeshFactory::generateAAPlaneXY( float width, float height, const std::string& materialName, Color4f color )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, height ),
									vec3f( 0.0f, 0.0f, 1.0f ),
									color,
									vec2f( 0.0f, 0.0f ) ) );

		vertices.push_back( Mesh::Vertex( vec3f( width, height ),
									vec3f( 0.0f, 0.0f, 1.0f ),
									color,
									vec2f( 1.0f, 0.0f ) ) );

		vertices.push_back( Mesh::Vertex( vec3f( width, 0.0f ),
									vec3f( 0.0f, 0.0f, 1.0f ),
									color,
									vec2f( 1.0f, 1.0f ) ) );
		
		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, 0.0f ),
									vec3f( 0.0f, 0.0f, 1.0f ),
									color,
									vec2f( 0.0f, 1.0f ) ) );
		
		
		indices.push_back( 0 );
		indices.push_back( 2 );
		indices.push_back( 3 );
		
		indices.push_back( 2 );
		indices.push_back( 0 );
		indices.push_back( 1 );

		return new Mesh( vertices, indices, materialName );
	}
	//-------------------------------------------------------------------------------------------------------------------------
	Mesh* MeshFactory::generateAAPlaneXZ( float width, float height, const std::string& materialName, Color4f color )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, 0.0f, height ),
									vec3f( 0.0f, 1.0f, 0.0f ),
									color,
									vec2f( 0.0f, 1.0f ) ) );

		vertices.push_back( Mesh::Vertex( vec3f( width, 0.0f, height ),
									vec3f( 0.0f, 1.0f, 0.0f ),
									color,
									vec2f( 1.0f, 1.0f ) ) );

		vertices.push_back( Mesh::Vertex( vec3f( width, 0.0f, 0.0f ),
									vec3f( 0.0f, 1.0f, 0.0f ),
									color,
									vec2f( 1.0f, 0.0f ) ) );
		
		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, 0.0f, 0.0f ),
									vec3f( 0.0f, 1.0f, 0.0f ),
									color,
									vec2f( 0.0f, 0.0f ) ) );
		
		
		indices.push_back( 0 );
		indices.push_back( 3 );
		indices.push_back( 2 );
		
		indices.push_back( 2 );
		indices.push_back( 1 );
		indices.push_back( 0 );

		return new Mesh( vertices, indices, materialName );
	}
	//-------------------------------------------------------------------------------------------------------------------------
	Mesh* MeshFactory::generateAAPlaneYZ( float width, float height, const std::string& materialName, Color4f color )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, 0.0f, height ),
									vec3f( 1.0f, 0.0f, 0.0f ),
									color,
									vec2f( 0.0f, 1.0f ) ) );

		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, width, height ),
									vec3f( 1.0f, 0.0f, 0.0f ),
									color,
									vec2f( 1.0f, 1.0f ) ) );

		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, width, 0.0f ),
									vec3f( 1.0f, 0.0f, 0.0f ),
									color,
									vec2f( 1.0f, 0.0f ) ) );
		
		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, 0.0f, 0.0f ),
									vec3f( 1.0f, 0.0f, 0.0f ),
									color,
									vec2f( 0.0f, 0.0f ) ) );
		
		
		indices.push_back( 0 );
		indices.push_back( 2 );
		indices.push_back( 3 );
		
		indices.push_back( 2 );
		indices.push_back( 0 );
		indices.push_back( 1 );

		return new Mesh( vertices, indices, materialName );	
	}

	Mesh* MeshFactory::generateLine( const vec3f& start, const vec3f& end, const std::string& materialName, Color4f color )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		vertices.push_back( Mesh::Vertex( start, vec3f(), color, vec2f() ) );
		vertices.push_back( Mesh::Vertex( end, vec3f(), color, vec2f() ) );

		indices.push_back( 0 );
		indices.push_back( 1 );

		return new Mesh( vertices, indices, materialName, GL_LINES );
	}
	//----------------------------------------------------------------------------------------------
	/* Generates along the +x-axis */
	Mesh* MeshFactory::generateCone( float length, float angleFromCenterRadians, int numSectors, const std::string& materialName, Color4f color )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		float zyRadius = length * std::tan( angleFromCenterRadians );
		float angleDelta = 2.0f * MathFuncs<float>::PI / numSectors;
		float startX = 0.0f;
		float endX = startX + length;

		float currentAngle = 0.0f;
		for( int s = 0; s <= numSectors*3; ++s )
		{
			float y = zyRadius * std::sin( currentAngle );
			float z = zyRadius * std::cos( currentAngle );

			vertices.push_back( Mesh::Vertex(	vec3f( endX, y, z ),
												vec3f(),
												color,
												vec2f() ) );
			
			currentAngle += angleDelta;
			y = zyRadius * std::sin( currentAngle );
			z = zyRadius * std::cos( currentAngle );

			vertices.push_back( Mesh::Vertex(	vec3f( endX, y, z ),
												vec3f(),
												color,
												vec2f() ) );
			
			vertices.push_back( Mesh::Vertex(	vec3f( startX, 0.0f, 0.0f ),
												vec3f(),
												color,
												vec2f() ) );
			
			indices.push_back( s + 2 );
			indices.push_back( s + 1 );
			indices.push_back( s );
			s += 2;
		}
		return new Mesh( vertices, indices, materialName );
	}
	
	void drawTriangleForIcosahedron( vec3f& v1, vec3f& v2, vec3f& v3, float radius, std::vector< Mesh::Vertex >& vertices, std::vector< unsigned int >& indices, const Color4f& color );
	void subdivideForIcosahedron( vec3f& v1, vec3f& v2, vec3f& v3, float radius, std::vector< Mesh::Vertex >& vertices, std::vector< unsigned int >& indices, const Color4f& color, int depth );
	vec2f getTexCoordsForIcosahedron( const vec3f& normal );

	Mesh* MeshFactory::generateIcosahedron( float radius, int subdivisions, const std::string& materialName, Color4f color )
	{
		const float X = 0.525731112119133606f;
		const float Z = 0.850650808352039932f;

		static vec3f vdata[12] = { 
			vec3f( -X, 0.f, Z ), vec3f( X, 0.f, Z ), vec3f( -X, 0.f, -Z ), vec3f( X, 0.f, -Z ),
			vec3f( 0.f, Z, X ), vec3f(0.f, Z, -X ), vec3f( 0.f, -Z, X ), vec3f( 0.f, -Z, -X ),
			vec3f( Z, X, 0.f ), vec3f( -Z, X, 0.f ), vec3f( Z, -X, 0.f ), vec3f( -Z, -X, 0.f ) };

		static unsigned int tindices[20][3] = {
			{0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},    
			{8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},    
			{7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6}, 
			{6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };


		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		for( int i = 0; i < 20; ++i )
		{
			subdivideForIcosahedron(	vdata[tindices[i][0]],
										vdata[tindices[i][1]],
										vdata[tindices[i][2]], radius, vertices, indices, color, subdivisions );
		}
		
		return new Mesh( vertices, indices, materialName, GL_TRIANGLES );
	}

	void drawTriangleForIcosahedron( vec3f& v1, vec3f& v2, vec3f& v3, float radius, std::vector< Mesh::Vertex >& vertices, std::vector< unsigned int >& indices, const Color4f& color )
	{
		vec2f texCoord1 = getTexCoordsForIcosahedron( v1 );
		vec2f texCoord2 = getTexCoordsForIcosahedron( v2 );
		vec2f texCoord3 = getTexCoordsForIcosahedron( v3 );
		vertices.push_back( Mesh::Vertex( v1 * radius, v1, color, texCoord1 ) );
		vertices.push_back( Mesh::Vertex( v2 * radius, v2, color, texCoord2 ) );
		vertices.push_back( Mesh::Vertex( v3 * radius, v3, color, texCoord3 ) );
		indices.push_back( indices.size() );
		indices.push_back( indices.size() );
		indices.push_back( indices.size() );
	}

	void subdivideForIcosahedron( vec3f& v1, vec3f& v2, vec3f& v3, float radius, std::vector< Mesh::Vertex >& vertices, std::vector< unsigned int >& indices, const Color4f& color, int depth )
	{
		vec3f v12, v23, v31;

		if( depth == 0 )
		{
			drawTriangleForIcosahedron( v1, v2, v3, radius, vertices, indices, color );
			return;
		}

		v12 = v1 + v2;
		v23 = v2 + v3;
		v31 = v3 + v1;

		v12.normalize();
		v23.normalize();
		v31.normalize();
		subdivideForIcosahedron( v1, v12, v31, radius, vertices, indices, color, depth - 1 );
		subdivideForIcosahedron( v2, v23, v12, radius, vertices, indices, color, depth - 1 );
		subdivideForIcosahedron( v3, v31, v23, radius, vertices, indices, color, depth - 1 );
		subdivideForIcosahedron( v12, v23, v31, radius, vertices, indices, color, depth - 1 );
	}

	Monky::vec2f getTexCoordsForIcosahedron( const vec3f& normal )
	{
		vec2f texCoords;
		float normalizedX = 0;
		float normalizedZ = -1;

		float xzSquared = normal.x * normal.x + normal.z * normal.z; 
		if( xzSquared > 0 )
		{
			normalizedX = std::sqrt( ( normal.x * normal.x ) / ( xzSquared ) );
			if( normal.x < 0 )
			{
				normalizedX = -normalizedX;
			}

			normalizedZ = sqrt( ( normal.z * normal.z ) / ( xzSquared ) );
			if( normal.z < 0 )
			{
				normalizedZ = -normalizedZ;
			}

			if( normalizedZ == 0 )
			{
				texCoords.x = ( ( normalizedX * MathFuncs<float>::PI ) * 0.5f );
			}
			else
			{
				texCoords.x = atan( normalizedX / normalizedZ );
				if( normalizedZ < 0 )
				{
					texCoords.x += MathFuncs<float>::PI;
				}				
			}
			if( texCoords.x < 0 )
			{
				texCoords.x += 2 * MathFuncs<float>::PI;
			}
			texCoords.x /= 2 * MathFuncs<float>::PI;
			texCoords.y = ( -normal.y + 1 ) * 0.5f;
		}


		return texCoords;
	}



}