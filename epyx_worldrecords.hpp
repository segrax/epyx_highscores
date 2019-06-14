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
