#pragma once
#include "CoreEngine/CommonCoreEngineIncludes.h"

namespace Monky
{
	class TileSet
	{

	public:
		TileSet( int firstGid, const std::string& name, int tileWidth, int tileHeight, const std::string& textureFile, int imageWidth, int imageHeight );

		bool IsInTileSet( int gid );
		int GetNormalizedGIDInTileSet( int gid );
		vec2f GetTopLeftTexCoords( int normalizedGID );

		vec2f GetTileSizeInUVSpace() const { return m_tileSizeInUVSpace; }
		const std::string& GetMaterial() const { return m_material; }
		const std::string& GetName() const { return m_name; }

	private:
		int m_firstGid;
		int m_lastGid;
		std::string m_name;
		int m_tileWidth;
		int m_tileHeight;
		std::string m_material;
		int m_imageWidth;
		int m_imageHeight;
		int m_tileAmountWidth;
		vec2f m_tileSizeInUVSpace;
	};
}
