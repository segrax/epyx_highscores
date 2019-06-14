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

enum eGames {
	eGAME_SUMMER,
	eGAME_SUMMER2,
	eGAME_WINTER,
	eGAME_WORLD,
	eGAME_CALIFORNIA
};

struct sKnownDisk {
	std::string mLabel;					// Disk Label
	std::string mRecordFile;			// WorldRecord filename
};

struct sKnownGame {
	eGames mGameID;						// ID of game
	std::string mName;					// Title of game

	std::vector<sKnownDisk> mDisks;		// Known D64s
	std::vector<std::string> mEvents;	// Name of events
	std::vector<size_t> mEventSorting;	// Sorting mechanism for records for events
};

struct sParameters {
	bool mAbout;
	bool mHelp;
	bool mList;	
	bool mListAll;			// List all records
	bool mListKnownGames;	// Display known games list

	std::string mFilterGame;// Filter records for this game
	std::string mFilterName;// Filter records for this name 

	std::string mImportPath;// Path to import from D64/CRT
	std::string mImportCart;// Path to import from a specific CRT

	/**
	 * Constructor: Set parameters to defaults
	 */
	sParameters() {
		mAbout = false;
		mHelp = false;
		mList = false;
		mListAll = false;
		mListKnownGames = false;
	}
};
