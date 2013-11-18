#pragma once
#include "CoreEngine/StdLibraryIncludes.h"

namespace Monky
{
	class BufferLayout
	{
	public:
		enum DataType
		{
			INT = 0,
			INT2,
			INT3,
			INT4,
			BYTE,
			FLOAT1,
			FLOAT2,
			FLOAT3,
			FLOAT4
		};

		BufferLayout();
		BufferLayout( int numAttribs, ... );

		void pushAttribute( DataType dt );
		unsigned int numAttributes() const;
		unsigned int stride() const;
		int dataTypeSize( DataType dt ) const;
		int attributeSize( unsigned int i ) const;
		int attributeOffset( unsigned int index ) const;
		int numOfDataType( DataType dt ) const;
		int numOfDataTypeForAttrib( unsigned int i ) const;

		int getGLType( int i ) const;

	private:
		unsigned int m_stride;
		std::vector< DataType > m_layout;
	};
}
