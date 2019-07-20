/*

Copyright (c) 2019 Robert Crossfield

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/


#include "stdafx.hpp"
#include <algorithm>

// Function to rip a std::string from a uint8_t array
std::string stringRip( uint8_t *pBuffer, uint8_t pTerminator, size_t pLengthMax) {
	std::string tmpString;

	for(size_t i = 0; *pBuffer != pTerminator && i <= pLengthMax; ++i)
		tmpString += (char) *pBuffer++;

	return tmpString;
}

// D64 Constructor
cD64::cD64( std::string pD64, bool pCreate, bool pReadOnly ) {

	// Prepare variables
	mCreated = false;
	mReady = false;
	mLastOperationFailed = false;

	mRead = pReadOnly;

	// Filename, Use datapath mode
	mFilename = pD64;

	// Number of tracks
	mTrackCount = 35;

	bamClear();

	// Read the file
	mBuffer = gResources->FileRead( pD64 );

	// Loaded size is 0 and we're not in create mode?
	if( !mBuffer->size() && !pCreate )
		return;

	// Creating a file?
	if( !mBuffer->size() ) {
		mRead = false;

		// Create a new 35 Trackdisk
		mBuffer = std::make_shared<std::vector<uint8_t>>();
		mBuffer->resize(0x2AB00, 0);

		// Create it
		//if( local_FileCreate( pD64, mPath, pDataSave ) == false ) 
		//	return;

		// Build a BAM
		bamCreate();

		// Write out the new disk
		diskWrite();

		mCreated = true;

	} else
		// Load the BAM
		bamLoadFromBuffer();

	// Read the disk directory
	directoryLoad();

	mReady = true;

	//diskTest();
}

// D64 Destructor
cD64::~cD64( ) {
	
	// Write out disk changes
	diskWrite();
	
	// Cleanup loaded files memory
	filesCleanup();
}

// Check the BAM against the loaded information
bool cD64::bamTest( ) {

	for( uint8_t track = 1; track <= mTrackCount; ++track )

		// Check each sector of the track
		for( uint8_t sector = 0; sector < trackRange( track ); ++sector ) {
		
			if( mBamTracks[track][sector] )
				if( !mBamRealTracks[track][sector] )
					continue;

			if( !mBamTracks[track][sector] )
				if( mBamRealTracks[track][sector] )
					continue;

			return false;
		}

	return true;
}

// Create the BAM Track
void cD64::bamCreate() {
	const char *diskname = "CREEP SAVE";
	
	uint8_t *buffer = sectorPtr( 18, 0 );

	// Track / Sector of First Directory Sector
	buffer[0x00] = 0x12;		// Track 18
	buffer[0x01] = 0x01;		// Sector 1

	// Dos Version Type
	buffer[0x02] = 0x41;
	
	// Unused Areas
	buffer[0xA4] = 0xA0;
	buffer[0xA5] = 0x2A;
	buffer[0xA6] = 0x2A;
	buffer[0xA0] = buffer[0xA1] = 0xA0;
	buffer[0xA7] = buffer[0xA8] = buffer[0xA9] = buffer[0xAA] = 0xA0;
	
	// Disk Name
	memset( &buffer[0x90], 0xA0, 0x10 );
	memcpy( &buffer[0x90], diskname, strlen( diskname ) );

	// Disk ID
	buffer[0xA2] = 0x00;	buffer[0xA3] = 0x00;
	
	// Clear the BAM to empty disk state
	bamClear();

	// Mark both Track 18 sectors 0/1 as in use
	bamSectorMark( 18, 0, false );
	bamSectorMark( 18, 1, false );

	// Save the new bam to the buffer
	bamSaveToBuffer();
}

// Set BAM to empty disk state
void cD64::bamClear() {
	uint8_t range = 0;

	// Clear BAM memory 
	for( uint8_t T = 0; T <= mTrackCount; ++T ) {
		mBamFree[T] = range = trackRange(T);

		for( uint8_t S = 0; S <= 0x17; ++S ) {
			mBamRealTracks[T][S] = 0;

			if(S < range) {
				
				mBamTracks[T][S] = true;
			} else {
				mBamTracks[T][S] = false; 
			}
		}
	}
}

// Load the internal 'mBamTracks' flags from the buffer
void cD64::bamLoadFromBuffer() {
	uint8_t *buffer = sectorPtr( 18, 0 );

	bamClear();

	// Load the BAM
	buffer += 0x04;

	for( size_t T = 1; T <= mTrackCount; ++T ) {
		// Free Sector Count
		mBamFree[T] = *buffer++;
			
		// Load each sector
		for( size_t S = 0; S <= 0x17; ) {

			uint8_t data = *buffer++;

			// Load the value for each sector in this byte
			for( uint8_t count = 0; count < 8; ++count, ++S ) {
				if( data & 0x01 )
					mBamTracks[T][S] = true;
				else
					mBamTracks[T][S] = false;

				data >>= 1;
			}
		}
	}

}

// Save the BAM from the internal 'mBamTracks' to the buffer
void cD64::bamSaveToBuffer() {
	uint8_t *buffer = sectorPtr( 18, 0 );

	buffer += 4;

	// Loop each track
	for( size_t track = 1; track <= mTrackCount; ++track ) {
		size_t sectors = trackRange( track );

		// Set number of free sectors
		buffer[0] = mBamFree[track];
		++buffer;

		// Loop each sector on this track
		for( size_t sector = 0; sector < sectors; ) {
			uint8_t final = 0;

			// Loop each sector 
			for( size_t count = 0; count < 8; ++count, ++sector ) {
				final >>= 1;

				// Mark it free if its free
				if( mBamTracks[track][sector] == true )
					final |= 0x80;
			}

			// Save this uint8_t of sector BAM
			*buffer++ = final;	
		}
	}
}

// Mark a sector as being Used/Free in the internal 'mBamTracks'
void cD64::bamSectorMark( size_t pTrack, size_t pSector, bool pValue ) {
	
	// Check if the sector was marked in use already
	if( mBamTracks[pTrack][pSector] == true ) {

		// Marked Free
		if( pValue == false )
			--mBamFree[pTrack];

	} else {
		// Marked in use
		if( pValue == true )
			++mBamFree[pTrack];
	}

	mBamTracks[pTrack][pSector] = pValue;
}

// Check a track for free sectors
bool cD64::bamTrackSectorFree( uint8_t &pTrack, uint8_t &pSector ) {
	uint8_t track = pTrack;
	bool wrapped = false;

	// Loop each sector in this track
	for( uint8_t sector = pSector; sector < trackRange( track ); ) {
			
		// Is this sector free
		if( mBamTracks[track][sector] == true ) {
			pTrack = track;
			pSector = sector;
			return true;
		}
	
		++sector;

		// Check its in range for the track
		if( sector >= trackRange( track ) ) {
			if( wrapped == false ) {
				wrapped = true;
				sector = 0;
				pSector = 0;
				continue;
			}
			
			return false;
		}	// If >= trackrange

	}	// For Sector

	return false;
}

// Find a free sector
// 
// Starting from the directory track, attempt to find a free sector, moving the head in one direction
//
bool cD64::bamSectorFree( uint8_t &pTrack, uint8_t &pSector, uint8_t pDirectoryTrack ) {
	bool first = false, moveDown = true;
	uint8_t moveSize = 0;
	
	// Disk track check has gone past the end atleast once
	bool wrapped = false;

	// No track specified, start near the directorytrack
	if(pTrack == 0) {
		pTrack = (pDirectoryTrack - 1);
		first = true;
	}

	// Get the distance between the directory track and the specified track
	if( pTrack < pDirectoryTrack ) {
		moveDown = true;
		moveSize = pDirectoryTrack - pTrack;
	} else {
		moveDown = false;
		moveSize = pTrack - pDirectoryTrack;
	}
	
	// Increase sector by the drive increment
	if(!first)
		pSector += 10;

	// Calculated sector has passed max for the track?
	if( pSector >= trackRange( pTrack ) ) {
		pSector = 0;
	}

	// Loop for each track of the disk
	for( uint8_t track = pTrack; track > 0 && track <= mTrackCount; ) {
		
		// Test this track, starting at 'pSector'
		if( bamTrackSectorFree( track, pSector ) == true ) {
			pTrack = track;
			return true;
		}

		// If we're trying to locate the first sector for a new 'file save'
		// Move around based on the directory track
		if(first) {
			if(moveDown) {
				moveDown = false;
				track += moveSize;
				if( track == pDirectoryTrack )		// DirectoryTrack is last resort
					++track;
			} else {
				moveDown = true;
				track -= moveSize;
				if( track == pDirectoryTrack )		// DirectoryTrack is last resort
					--track;
			}

			if( track == 0 || track > mTrackCount )
				break;
			
			moveSize += 1;
			pSector = 0;
			continue;
		}

		// Move the track down or up depending on current direction
		if(moveDown) {
			--track;
			if( track == pDirectoryTrack )		// DirectoryTrack is last resort
				--track;

		} else {
			++track;
			if( track == pDirectoryTrack )		// DirectoryTrack is last resort
				++track;
		}

		// Passed first track on disk?
		if( track == 0 ) {
			moveDown = false;
			track += (pDirectoryTrack + 1);
			if(wrapped)
				break;
			wrapped = true;
		}
		
		// At last track on disk?
		if( track > mTrackCount ) {
			moveDown = true;
			track = (pDirectoryTrack - 1);
			if(wrapped)
				break;
			wrapped = true;
		}
		
		// Start at first sector of track
		pSector = 0;

	}	// For Track

	// If all else fails, attempt to use a sector from the directory track
	pTrack = 0x12;
	pSector = 0x00;

	return bamTrackSectorFree( pTrack, pSector );
}

// Check a disk for errors
bool cD64::diskTest() {
	std::vector< sD64Chain >::iterator			 linkIT;
	if(!bamTest()) {

		// Bam Disk test failed
		//cout << "Disk has bad BAM" << endl;
		// Repair it based on loaded files
		//bamRebuild();

	}
	
	// TODO 
	// Check for cross linked files
	//for( linkIT = mCrossLinked.begin(); linkIT != mCrossLinked.end(); ++linkIT ) {
	//}

	return true;
}

// Load an entry
sD64File *cD64::directoryEntryLoad( uint8_t *pBuffer ) {
	sD64File *file = new sD64File();
			
	// Get the filetype
	file->mFileType = (eD64FileType) (pBuffer[0x02] & 0x0F);

	// Get the filename
	file->mName = stringRip( pBuffer + 0x05, 0xA0, 16 );
	if( file->mName.size() == 0 ) {
		delete file;
		return 0;
	}
	
	// Get the starting Track/Sector
	file->mTrack = pBuffer[0x03];
	file->mSector = pBuffer[0x04];
	
	// Total number of blocks
	file->mFileSize = readLEWord( &pBuffer[0x1E] );

	// Load the file into a new buffer
	if(file->mFileSize > 0)
		fileLoad( file );
		
	return file;
}

// Load the disk directory
void cD64::directoryLoad() {
	// Directory starts at Track 18, sector 1
	uint8_t   currentTrack = 0x12, currentSector = 1;
	uint8_t *sectorBuffer;

	// Cleanup previous load
	filesCleanup();

	// Loop until the current Track/Sector is invalid
	while( (currentTrack > 0 && currentTrack <= mTrackCount) && 
		(currentSector <= trackRange( currentTrack )) ) {

		sectorBuffer = sectorPtr( currentTrack, currentSector );
		uint8_t *buffer = sectorBuffer;
		
		if(!buffer)
			break;

		// 8 Entries per sector, 0x20 bytes per entry
		for( uint8_t i = 0; i <= 7; ++i, buffer += 0x20 ) {
			
			sD64File *file = directoryEntryLoad( buffer );
			
			if(file)
				mFiles.push_back( file );
		}

		// Get the next Track/Sector in the chain
		currentTrack = sectorBuffer[0];
		currentSector = sectorBuffer[1];
	}
}

// Set the directory entry in the buffer
bool cD64::directoryEntrySet( uint8_t pEntryPos, sD64File *pFile, uint8_t *pBuffer ) {
	if( pFile->mName.size() > 0x0F )
		return false;

	// Position zero means the T/S will have been set, and we dont want to overwrite it
	if( pEntryPos > 0)
		pBuffer[ 0x00 ] = pBuffer[ 0x01 ] = 0;
	
	// File Type
	pBuffer[ 0x02 ] = pFile->mFileType;
	
	// Starting Track/Sector
	pBuffer[ 0x03 ] = pFile->mTrack;
	pBuffer[ 0x04 ] = pFile->mSector;

	// Filename
	memset( &pBuffer[ 0x05 ], 0xA0, 0x10 );
	memcpy( &pBuffer[ 0x05 ], pFile->mName.c_str(), pFile->mName.size() );
	
	// Number of sectors used by file
	size_t totalBlocks = (pFile->mBuffer->size() / 254);
	if(pFile->mBuffer->size() % 254 )
		++totalBlocks;

	writeLEWord( &pBuffer[ 0x1E ], (uint16_t) totalBlocks );
	return true;
}

// Add file to directory
bool cD64::directoryAdd( sD64File *pFile ) {
	
	// Directory starts at Track 18, sector 1
	uint8_t   currentTrack = 0x12, currentSector = 1;
	uint8_t *sectorBuffer;

	// Loop until the current Track/Sector is invalid
	while( (currentTrack > 0 && currentTrack <= mTrackCount) && (currentSector <= trackRange( currentTrack )) ) {
		sectorBuffer = sectorPtr( currentTrack, currentSector );
		uint8_t *buffer = sectorBuffer;
		
		if(!buffer)
			break;

		// Is the next T/S set?
		if( sectorBuffer[0] == 0 ) {

			// Nope, so lets find the next available Track/Sector, and link it from this sector
			while( bamTrackSectorUse( currentTrack, currentSector ) == false ) {
				currentSector += 3;	
				
				// TODO: Handle this fail better
				if( currentSector > trackRange( currentTrack ) )
					return false;
			}

			// Found one, lets set it
			sectorBuffer[0] = currentTrack;
			sectorBuffer[1] = currentSector;

			// Mark it in use
			bamSectorMark( currentTrack, currentSector, false );
		}

		// Loop for 8 Entries, at 0x20 bytes per entry
		for( uint8_t i = 0; i <= 7; ++i, buffer += 0x20 ) {
			 uint8_t fileType = (*(buffer + 0x02) & 0x0F);

			 // Deleted Entries
			 if( fileType == 0x00 || fileType == 0x80 ) {

				 // Found free entry entry, overwrite it with the new file details
				directoryEntrySet( i, pFile, buffer );

				return true;
			 }
		}
		// This entry table is full
		
		// Get the next Track/Sector in the chain
		currentTrack = sectorBuffer[0];
		currentSector = sectorBuffer[1];
	};

	// No space remaining
	return false;
}

// Memory Cleanup
void cD64::filesCleanup() {
	std::vector< sD64File * >::iterator		fileIT;

	// Cleanup files
	for( fileIT = mFiles.begin(); fileIT != mFiles.end(); ++fileIT )
		delete *fileIT;

	mFiles.clear();
}

// Load a file from the disk
bool cD64::fileLoad( sD64File *pFile ) {
	uint16_t bytesCopied = 0, copySize = 0xFE;
	uint8_t currentTrack = pFile->mTrack, currentSector = pFile->mSector;
	bool   noCopy = false;

	// Cleanup old buffer
	pFile->mBuffer->clear();

	// Prepare the buffer, (Each block is 254 bytes, the remaining two is used for the T/S chain)
	pFile->mBuffer->resize(pFile->mFileSize * copySize);
	
	// Temp buffer ptr
	uint8_t *destBuffer = pFile->mBuffer->data();

	// Clear any previous chain information
	pFile->mTSChain.clear();

	// Loop until invalid track
	while( currentTrack  ) {
		
		// Get ptr to current sector
		uint8_t *buffer = sectorPtr( currentTrack, currentSector );

		if(!buffer) {

			// Reached an invalid track/sector! Abort if not noCopy mode,
			// Otherwise it will retry the entire read, and abort after reading up to this point
			// Of the file
			if( noCopy )
				break;
		
			pFile->mChainBroken = true;

			return false;
		}
	
		// Track/Sector already in use?
		if(mBamRealTracks[currentTrack][currentSector]) {

			// Add to the crosslinked list
			mCrossLinked.push_back( sD64Chain( currentTrack, currentSector, pFile ) );

		} else
			mBamRealTracks[currentTrack][currentSector] = pFile;

		pFile->mTSChain.push_back( sD64Chain( currentTrack, currentSector, pFile ) );

		// T/S is broken, or the directory entry is wrong about size
		if( bytesCopied >= pFile->mBuffer->size() )
			noCopy = true;

		// Last Sector of file? 
		if( buffer[0] == 0 ) {
			// Bytes used by the final sector stored in the T/S chain sector value
			copySize = (buffer[1] - 1);

			// Adjust bufer size to match the final file size
			pFile->mBuffer->resize( pFile->mBuffer->size() - (0xFE - copySize));
		}

		// Copy sector data, excluding the T/S Chain data
		if(!noCopy)
			memcpy( destBuffer, buffer + 2, copySize );
		
		// Move the dest buffer forward
		destBuffer += copySize;
		bytesCopied += copySize;

		// Next Track/Sector for this file
		currentTrack = buffer[0];
		currentSector = buffer[1];
	}

	// Retry file read using the new buffer size
	if(noCopy) {
		pFile->mFileSize = (bytesCopied / 254) + 1;
		
		// Do the retry
		return fileLoad( pFile );
	}

	return true;
}

// Save a file as a PRG to the disk
bool cD64::fileSave( std::string pFilename, uint8_t *pData, size_t pBytes, uint16_t pLoadAddress ) {
	size_t		bytesRemain = pBytes + 2;		// Add the Load Address Bytes
	sD64File	File;
	
	// Upper case only for C64 filenames
	std::transform( pFilename.begin(), pFilename.end(), pFilename.begin(), ::toupper );
	
	// Set the file details
	File.mName = pFilename;
	File.mFileType = (eD64FileType) 0x82;		// PRG

	//
	uint8_t *buffer = 0, *bufferSrc = pData;
	uint8_t  track = 0, sector = 0;
	size_t	 copySize = 0;

	bool sectorFirst = true;

	// Loop until there is no bytes left to save
	while( bytesRemain ) {
		// Get available T/S
		if( bamSectorFree( track, sector ) == false )
			return false;
		
		// Set the next T/S in the previous sector
		if(buffer) {
			buffer[0] = track;
			buffer[1] = sector;
		} else {
			File.mTrack = track;
			File.mSector = sector;
		}

		// Grab buffer to next sector
		buffer = sectorPtr( track, sector );
		// If its the first sector, we have to write the load address
		if( sectorFirst ) {
			writeLEWord( &buffer[0x02], pLoadAddress );
			// Copy filedata
			if(bytesRemain < 0xFC )
				copySize = bytesRemain;
			else		
				copySize = 0xFC;

			sectorFirst = false;

			// Copy the source to the disk buffer
			memcpy( buffer + 4, bufferSrc, copySize );
		} else {
			// Normal sector write
			// Copy filedata
			if(bytesRemain < 0xFE )
				copySize = bytesRemain;
			else		
				copySize = 0xFE;

			// Copy the source to the disk buffer
			memcpy( buffer + 2, bufferSrc, copySize );
		}
		
		// Move the source forward
		bufferSrc += copySize;

		// Mark the sector in use
		bamSectorMark( track, sector, false );
		
		if(bytesRemain > 0)
			bytesRemain -= copySize;
	};
	
	// Set the Track to none (to mark end of chain), and the sector the the number of bytes used
	buffer[0] = 0;
	buffer[1] = (uint8_t) (copySize - 1);

	// Add entry to the directory
	if( directoryAdd( &File ) == false )
		return false;

	return diskWrite();
}

// Obtain pointer to 'pTrack'/'pSector' in the disk buffer
uint8_t *cD64::sectorPtr( uint8_t pTrack, uint8_t pSector ) {
	uint32_t	currentTrack, currentRange;
	uint32_t	offset = 0;

	// Invalid track?
	if(pTrack <= 0 || pTrack > mTrackCount)
		return 0;

	// Invalid sector?
	if(pSector < 0 || pSector > trackRange(pTrack))
		return 0;

	// Loop through each track, and add up
	for(currentTrack = 1; currentTrack <= pTrack; currentTrack++) {
		currentRange = trackRange( currentTrack );

		// If we're not looking for this track
		if(currentTrack != pTrack) {

			//increase the offset to the end of the track
			offset += (256 * currentRange);
			continue;
		} else {
			//increase offset to the sector we're after
			offset += (256 * pSector);

			if( offset >= mBuffer->size() )
				return 0;

			// Return pointer
			return (mBuffer->data() + offset);
		}
	}

	return 0;
}

// Write the buffer to the D64
bool cD64::diskWrite() {
	
	// Dont save if the disk was opened read only, or the last operation failed
	if( mRead || mLastOperationFailed)
		return false;

	// Store the internal 'mBuffer'
	bamSaveToBuffer();

	//return local_FileSave( mFilename, mPath, mDataSave, mBuffer, mBufferSize );
	return false;
}

// Get a file
sD64File *cD64::fileGet( std::string pFilename ) {
	std::vector< sD64File* >::iterator fileIT;
	
	// Upper case names only
	std::transform( pFilename.begin(), pFilename.end(), pFilename.begin(), ::toupper );

	// Loop thro all files on disk for specific filename
	for( fileIT = mFiles.begin(); fileIT != mFiles.end(); ++fileIT ) {

		if( (*fileIT)->mName == pFilename )
			return *fileIT;
	}

	return 0;
}

// Get a file list, with all files starting with 'pFind'
std::vector< sD64File* > cD64::directoryGet( std::string pFind ) {
	std::vector< sD64File* > result;
	std::vector< sD64File* >::iterator	fileIT;
	
	// Strip any astrix
	size_t	pos = pFind.find("*");
	if(pos != std::string::npos)
		pFind = pFind.substr(0, pos);

	// Loop all files found in directory
	for( fileIT = mFiles.begin(); fileIT != mFiles.end(); ++fileIT ) {
		sD64File *file = *fileIT;

		// Skip deleted files
		if(file->mFileType == eD64FileType_DEL )
			continue;
		
		// Check if the current file matches the 'pFind' string
		if( file->mName.substr(0, pFind.length()) ==  pFind )
			result.push_back( file );
	}

	return result;
}

std::string cD64::disklabelGet() {
	uint8_t* buffer = sectorPtr(18, 0);
	if (!buffer)
		return "";
	return stringRip(&buffer[0x90], 0xA0, 0x10);
}