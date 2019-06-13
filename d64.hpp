/*
 *  The Castles of Dr. Creep 
 *  ------------------------
 *
 *  Copyright (C) 1997-2014 Robert Crossfield
 *
 *   Parts of this code came from my 'D64-Pro' (ported from VB though)
 *  
 *  ------------------------------------------
 *  Commodore 64 Disk Image Handling
 *  ------------------------------------------
 */
enum eD64FileType {
	eD64FileType_DEL = 0,
	eD64FileType_SEQ = 1,
	eD64FileType_PRG = 2,
	eD64FileType_USR = 3,
	eD64FileType_REL = 4,
	eD64FileType_UNK
};

struct sD64File;

struct sD64Chain {
	size_t		 mTrack, mSector;
	sD64File		*mFile;

	sD64Chain( size_t pTrack, size_t pSector, sD64File *pFile ) {
		mTrack = pTrack;
		mSector = pSector;
		mFile = pFile;
	}
};


struct sD64File {
	bool	  mChainBroken;				// File Chain Broken
	std::string mName;					// Name of the file
	size_t	  mTrack, mSector;			// Starting T/S of file
	size_t	  mFileSize;				// Number of blocks used by file
	eD64FileType mFileType;				// Type of file

	tSharedBuffer mBuffer;				// Copy of file

	std::vector< sD64Chain >		mTSChain;	// Track/Sectors used by file

	sD64File() {						// Constructor
		mChainBroken = false;

		mTrack = 0;
		mSector = 0;

		mBuffer = std::make_shared<std::vector<uint8_t>>();
		mFileSize = 0;
	}

	~sD64File() {						// Destructor

	}
};

class cD64 {
private:
	bool						 mBamTracks[36][24];							// Track/Sector Availability Map (first allocation is ignored
	unsigned char				 mBamFree[36];									// Number of free sectors per track

	sD64File					*mBamRealTracks[36][24];						// Information about disk loaded based on files loaded

	tSharedBuffer				 mBuffer;										// Disk image buffer
	size_t						 mTrackCount;
	
	bool						 mCreated;										// Was a file created
	bool						 mRead;											// Disk is read only
	bool						 mReady;
	bool						 mLastOperationFailed;

	std::vector< sD64Chain >	 mCrossLinked;									// Files which are cross linked

	std::vector< sD64File* >	 mFiles;										// Files in disk
	std::string 				 mFilename;										// Name of current D64


	void						 bamClear();									// Clear the internal BAM
	void						 bamCreate();									// Create Track 18/0
	void						 bamLoadFromBuffer();							// Load Free Tracks/Sectors
	void						 bamSaveToBuffer();								// Save the mBamTracks data to the buffer

	void						 bamSectorMark( size_t pTrack, size_t pSector, bool pValue = true );				// Mark a sector used/free

	bool						 bamTrackSectorFree(uint8_t &pTrack,uint8_t &pSector );									// Check for free sectors in a track
	bool						 bamSectorFree(uint8_t &pTrack,uint8_t &pSector,uint8_t pDirectoryTrack = 0x12 );		// Check for free sectors on the disk

	bool						 bamTest( );																		// Test the BAM against the real one

	void						 directoryLoad();																	// Load the disk directory
	sD64File					*directoryEntryLoad( unsigned char *pBuffer );												// Load an entry
	bool						 directoryAdd( sD64File *pFile );													// Add file to directory
	bool						 directoryEntrySet( unsigned char pEntryPos, sD64File *pFile, unsigned char *pBuffer );				// Set the directory entry in the buffer
	
	void						 filesCleanup();										// Memory Cleanup
	bool						 fileLoad( sD64File *pFile );							// Load a file from the disk



	inline bool					 bamTrackSectorUse( unsigned char pTrack, unsigned char pSector ) {	// Is 'pTrack' / 'pSector' free?
		if( pTrack == 0 || pTrack > mTrackCount || pSector > trackRange(pTrack) )
			return false;

		return mBamTracks[ pTrack ][ pSector ];
	}

public:
								 cD64( std::string  pD64,bool pCreate = false, bool pReadOnly = true );
								~cD64( );

	std::vector< sD64File* >	 directoryGet( std::string  pFind );		// Get a file list, with all files starting with 'pFind'
	std::vector< sD64File* >	*directoryGet();					// Get the file list
	
	bool						 diskTest();						// Check a disk for errors
	bool						 diskWrite();						// Write the buffer to the D64

	sD64File					*fileGet( std::string  pFilename );		// Get a file
	bool						 fileSave( std::string  pFilename, unsigned char *pData, size_t pBytes, uint16_t pLoadAddress );// Save a file to the disk

	inline size_t				 sectorsFree() {					// Number of free sectors on disk
		size_t result = 0;

		for(size_t i = 1; i <= mTrackCount; ++i )
			result += mBamFree[i];
		return result;
	}

	uint8_t* sectorPtr(uint32_t pTrack, uint32_t pSector);				// Obtain pointer to 'pTrack'/'pSector' in the disk buffer
	
	inline size_t	 trackRange(const size_t pTrack) const {				// Number of sectors in 'pTrack'
		return 21 - (pTrack > 17) * 2 - (pTrack > 24) - (pTrack > 30);
	}

	inline size_t	trackCount() const {
		return mTrackCount;
	}

	inline bool					 createdGet() {						// Was file created
		return mCreated;
	}


	std::string					disklabelGet();
};
