#pragma once

#include "Colors.h"
#include "Material.h"
#include "CommonTypeDefines.h"
#include "StdLibraryIncludes.h"

// Note to prevent having to include openGL the default values were hardcoded :)
//GL_TRIANGLE = 4
//GL_STATIC_DRAW = 0x88E4

namespace Monky
{
	class VtxBufferAccessor;
	class GLBuffer;
	class BufferAccessor;
	class BufferLayout;

	class Mesh
	{
	public:

		enum MeshType
		{
			STATIC_MESH,
			SKELETAL_MESH
		};

		struct Vertex
		{
			Vertex(){}

			Vertex( float x, float y, float z,
					float xn, float yn, float zn,
					float r, float g, float b, float a,
					float u, float v, float xt = 0.0f, float yt = 0.0f,
					float zt = 0.0f, float xb = 0.0f, float yb = 0.0f, float zb = 0.0f )
					:	x( x )
					,	y( y )
					,	z( z )
					,	xn( xn )
					,	yn( yn )
					,	zn( zn )
					,	r( r )
					,	g( g )
					,	b( b )
					,	a( a )
					,	u( u )
					,	v( v )
					,	tx( xt )
					,	ty( yt )
					,	tz( zt )
					,	bx( xb )
					,	by( yb )
					,	bz( zb )
			{}

			Vertex( vec3f pos, vec3f norm, Color4f color, vec2f uv,
				vec3f tan = vec3f(), vec3f bit = vec3f() )
				:	x( pos.x )
				,	y( pos.y )
				,	z( pos.z )
				,	xn( norm.x )
				,	yn( norm.y )
				,	zn( norm.z )
				,	r( color.r )
				,	g( color.g )
				,	b( color.b )
				,	a( color.a )
				,	u( uv.x )
				,	v( uv.y )
				,	tx( tan.x )
				,	ty( tan.y )
				,	tz( tan.z )
				,	bx( bit.x )
				,	by( bit.y )
				,	bz( bit.z )
			{}
			float x, y, z;
			float xn, yn, zn;
			float r, g, b, a;
			float u, v;
			float tx, ty, tz;
			float bx, by, bz;
		};

		struct TriangleBatch
		{
			TriangleBatch()
				:	vboAccessor( nullptr )
				,	iboAccessor( nullptr )
				,	vboBuffer( nullptr )
				,	iboBuffer( nullptr )
			{}
			TriangleBatch( VtxBufferAccessor* vtxAccessor, BufferAccessor* iboAccessor,
				GLBuffer* vbo, GLBuffer* ibo, const std::string& mat )
				:	vboAccessor( vtxAccessor )
				,	iboAccessor( iboAccessor )
				,	vboBuffer( vbo )
				,	iboBuffer( ibo )
				,	material( mat )
			{}
			~TriangleBatch();
			VtxBufferAccessor*	vboAccessor;
			BufferAccessor*		iboAccessor;
			GLBuffer*			vboBuffer;
			GLBuffer*			iboBuffer;
			std::string			material;
		};

		Mesh( TriangleBatch* triBatch );
		Mesh( GLBuffer* vboBuffer, GLBuffer* iboBuffer, VtxBufferAccessor* vtxAccessor, const std::string& material, bool rvsWinding = false );

		Mesh( std::vector< Vertex >& vertices, const std::string& material, int mode = 4, int bufferType = 0x88E4 );	
		Mesh( std::vector< Vertex >& vertices, std::vector< unsigned int >& indices, const std::string& material, int mode = 4, int bufferType = 0x88E4 );
		~Mesh();

		void setIndices( std::vector< unsigned int >& indices, int triBatch = 0, int mode = 4, int bufferType = 0x88E4 );
		void setMaterial( const std::string& material, size_t i = 0 );
		
		const std::string& getMaterial( size_t i = 0 );
		const unsigned int getVBOID( size_t i = 0 );
		const unsigned int getIBOID( size_t i = 0 );
		const int getNumVertices( size_t i = 0 );
		const int getNumIndices( size_t i = 0 );
		const BufferLayout* getBufferLayout( size_t i = 0 );
		const GLBuffer* getVBOBuffer( size_t i = 0 );
		const int getRenderMode( size_t i = 0 );
		
		void setDebugMaterial( const std::string& debugMaterial ) { m_debugMaterial = debugMaterial; }
		const std::string& getDebugMaterial() { return m_debugMaterial; }
		
		TriangleBatch* getTriangleBatch( size_t i );
		size_t getNumTriBatches() { return m_triangleBatches.size(); }
		void pushTriangleBatch( TriangleBatch* triBatch );
		void pushTriangleBatch( GLBuffer* vboBuffer, GLBuffer* iboBuffer, VtxBufferAccessor* vtxAccessor, const std::string& material, bool rvsWinding = false );

		void reverseWinding( TriangleBatch* triBatch );
		MeshType getMeshType() const { return m_meshType; }

#pragma warning( disable:4100 )
		virtual void updateMaterial( const matStackf& modelStack, int index ){};
#pragma warning( default:4100 )

		void addLight( const Light* light, int i );
		
	protected:
		void generateTangentSpaceVectors( std::vector< Vertex >& vertices, std::vector< unsigned int >& indices );
		void generateTangentSpaceVectors( std::vector< Vertex >& vertices );
		void computeTangentSpaceMatrix( Vertex& p0, Vertex& p1, Vertex& p2 );
		TriangleBatch* buildTriangleBatch( GLBuffer* vboBuffer, GLBuffer* iboBuffer, VtxBufferAccessor* vtxAccessor, const std::string& material );
		TriangleBatch* buildTriangleBatch( std::vector< Vertex >& vertices, const std::string& material, int mode = 4, int bufferType = 0x88E4 );
		TriangleBatch* buildTriangleBatch( std::vector< Vertex >& vertices, std::vector< unsigned int >& indices, const std::string& material, int mode = 4, int bufferType = 0x88E4 );

	protected:
	
		std::string m_debugMaterial;
		std::vector< TriangleBatch* > m_triangleBatches;
		MeshType m_meshType;
	};
}