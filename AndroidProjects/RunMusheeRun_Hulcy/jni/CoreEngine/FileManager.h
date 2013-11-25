#pragma once
#include "Uncopyable.h"
#include "CommonCoreEngineIncludes.h"

class AAssetManager;

namespace Monky
{
	class FileManager : public Uncopyable
	{
	public:
		enum ReadMode
		{
			DATA_FOLDER_ONLY,
			DATA_ARCHIVE_ONLY,
			DATA_FOLDER_THEN_ARCHIVE,
			DATA_ARCHIVE_THEN_FOLDER
		};

		FileManager();
		~FileManager();

		//void initialize( const std::string& rootDirectory, const std::string& zipArchiveFile, ReadMode readMode, const std::string& password = "" );
		void initialize( AAssetManager* assetManager );
		void setReadMode( ReadMode readMode );
		char* readDataFromFile( const std::string& filePathRelToRootDir, int& bufferSize ) const;
		void writeDataToFile( const std::string& filePathRelToRootDir, char* data, int size ) const;

		const std::string& getRootDir() const;

	protected:
		char* readFileFromZipFile( const std::string& filePathRelToRootDir, int& bufferSize ) const;
		char* readFileFromUnZipFile( const std::string& filePathRelToRootDir, int& bufferSize ) const;

	private:
		AAssetManager* m_assetManager;
		std::string m_rootDir;
		ReadMode m_readMode;
		bool m_isInitialized;
	};

	FileManager& getFileManager();
}
