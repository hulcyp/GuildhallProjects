#include "FileManager.h"
//#include "zip.h"
//#include "unzip.h"
#include <android/asset_manager.h>
#include "CoreEngine/Logging.h"


namespace Monky
{
	FileManager g_fileManager;
	//MONKY_CRITICAL_SECTION g_fileManagerCS;
	//--------------------------------------------------------------
	FileManager& getFileManager()
	{
		return g_fileManager;
	}
	//--------------------------------------------------------------
	FileManager::FileManager()
	{}
	//--------------------------------------------------------------
	FileManager::~FileManager()
	{}
	//--------------------------------------------------------------
	void FileManager::initialize( AAssetManager*  assetManager )
	{
		if( !m_isInitialized )
		{
			m_assetManager = assetManager;
			//Monky::initializeCriticalSection( g_fileManagerCS );
			m_isInitialized = true;
		}
	}
	//--------------------------------------------------------------
	char* FileManager::readDataFromFile( const std::string& filePathRelToRootDir, int& bufferSize ) const
	{
		char* buffer = nullptr;

		if( m_isInitialized )
		{
			AAsset* assetFile = AAssetManager_open( m_assetManager, filePathRelToRootDir.c_str(), AASSET_MODE_BUFFER );
			if( assetFile != nullptr )
			{
				bufferSize = AAsset_getLength( assetFile );
				if( bufferSize != -1 )
				{
					consolePrintf( "File size: %d", bufferSize );
					buffer = (char*) malloc( bufferSize + 1 );
					AAsset_read( assetFile, buffer, bufferSize );
					buffer[bufferSize] = 0;
					AAsset_close( assetFile );
				}
			}
			else
				bufferSize = -1;
		}
		return buffer;
	}
	//--------------------------------------------------------------
	void FileManager::setReadMode( ReadMode readMode )
	{
		m_readMode = readMode;
	}
	//--------------------------------------------------------------
	void FileManager::writeDataToFile( const std::string& filePathRelToRootDir, char* data, int size ) const
	{
		if( size > 0 && m_isInitialized )
		{

		}
	}
	//--------------------------------------------------------------
	const std::string& FileManager::getRootDir() const
	{
		return m_rootDir;
	}
	//--------------------------------------------------------------
	char* FileManager::readFileFromZipFile( const std::string& filePathRelToRootDir, int& bufferSize ) const
	{
		char* buffer = nullptr;

		return buffer;	
	}
	//--------------------------------------------------------------
	char* FileManager::readFileFromUnZipFile( const std::string& filePathRelToRootDir, int& bufferSize ) const
	{
		char* buffer = nullptr;
		return buffer;
	}
#pragma warning( default:4355 )
}
