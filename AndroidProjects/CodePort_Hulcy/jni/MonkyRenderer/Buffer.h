#pragma once

namespace Monky
{
	class BufferLayout;

	class Buffer
	{
	public:
		Buffer( const BufferLayout* layout );
		Buffer( const BufferLayout* layout, unsigned int count );
		Buffer( char* data, unsigned int count, const BufferLayout* layout );
		virtual ~Buffer();

		void setData( char* data, unsigned int count );
		char* rawData();
		unsigned int size();
		unsigned int count();
		unsigned int stride();
		char* at( unsigned int i );
		const BufferLayout* getBufferLayout();

	protected:
		char* m_buffer;
		unsigned int m_size;
		const BufferLayout* m_bufferLayout;
	};
}
