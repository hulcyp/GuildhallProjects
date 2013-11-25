#include "Buffer.h"
#include "CoreEngine/CommonCoreEngineIncludes.h"
#include "BufferLayout.h"

namespace Monky
{
	Buffer::Buffer( const BufferLayout* layout )
		:	m_size( 0 )
		,	m_bufferLayout( layout )
		,	m_buffer( nullptr )
	{}

	Buffer::Buffer( const BufferLayout* layout, unsigned int count )
		:	m_size( layout->stride() * count )
		,	m_bufferLayout( layout )
	{
		m_buffer = new char[ m_size ];
	}

	Buffer::Buffer( char* data, unsigned int count, const BufferLayout* layout )
		:	m_buffer( data )	
		,	m_size( layout->stride() * count )
		,	m_bufferLayout( layout )
	{}

	Buffer::~Buffer()
	{
		SAFE_ARRAY_DELETE( m_buffer );
		SAFE_DELETE( m_bufferLayout );
	}

	void Buffer::setData( char* data, unsigned int count )
	{
		m_buffer = data;
		m_size = m_bufferLayout->stride() * count;
	}

	char* Buffer::rawData()
	{
		return m_buffer;
	}

	unsigned int Buffer::size()
	{
		return m_size;
	}
	
	unsigned int Buffer::count()
	{
		return m_size / m_bufferLayout->stride();
	}

	unsigned int Buffer::stride()
	{
		return m_bufferLayout->stride();
	}

	char* Buffer::at( unsigned int i )
	{
		assertion( i < count(), "Buffer index %d is out of bounds", i );
		return m_buffer + i * m_bufferLayout->stride();
	}

	const BufferLayout* Buffer::getBufferLayout()
	{
		return m_bufferLayout;
	}
}
