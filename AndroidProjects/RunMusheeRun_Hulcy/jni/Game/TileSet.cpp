#include "TileSet.h"
#include "MonkyRenderer/Material.h"
#include "MonkyRenderer/Texture.h"

namespace Monky
{
	TileSet::TileSet( int firstGid, const std::string& name, int tileWidth, int tileHeight, const std::string& textureFile, int imageWidth, int imageHeight )
	{
		m_firstGid = firstGid;
		m_name = name;
		m_tileWidth = tileWidth;
		m_tileHeight = tileHeight;
		Texture* tex = Texture::CreateOrGetTexture( textureFile, "textures/" + textureFile );
		consolePrintf( "Texture for tileset loaded..." );
		Material* mat = Material::createOrGetMaterial( textureFile, "simpleShader" );

		//mat->addUniform( "uUseDiffuseMap", 0 );

		if( tex != nullptr )
		{
			mat->addTexture( "uDiffuseMap", tex );
			mat->addUniform( "uUseDiffuseMap", 1 );
			consolePrintf( "Material created for tileset..." );
		}

		m_material = textureFile;
		m_imageWidth = imageWidth;
		m_imageHeight = imageHeight;
		m_tileAmountWidth = (int)( imageWidth / (float)tileWidth );
		m_lastGid = m_tileAmountWidth * (int)( imageHeight / (float)tileHeight ) + firstGid;
		m_tileSizeInUVSpace.x = tileWidth / (float)m_imageWidth;
		m_tileSizeInUVSpace.y = tileHeight / (float)m_imageHeight;
		consolePrintf( "Tileset loaded: %s", name.c_str() );
	}


	bool TileSet::IsInTileSet( int gid )
	{
		return m_firstGid <= gid && gid <= m_lastGid;
	}

	int TileSet::GetNormalizedGIDInTileSet( int gid )
	{
		return gid - m_firstGid;
	}

	vec2f TileSet::GetTopLeftTexCoords( int normalizedGID )
	{
		int xIdx = normalizedGID % m_tileAmountWidth;
		int yIdx = (int) ( normalizedGID / (float)m_tileAmountWidth );

		return vec2f( xIdx, yIdx ) * m_tileSizeInUVSpace;
	}
}
