enum eGames {
	eGAME_SUMMER,
	eGAME_SUMMER2,
	eGAME_WINTER,
	eGAME_WORLD,
	eGAME_CALIFORNIA
};

struct sKnownDisk {
	std::string mLabel;
	std::string mRecordFile;
};

struct sKnownGame {
	eGames mGameID;
	std::string mName;

	std::vector<sKnownDisk> mDisks;
	std::vector<std::string> mEvents;
	std::vector<size_t> mEventSorting;
};

struct sParameters {
	bool mAbout;
	bool mHelp;
	bool mList;	
	bool mListAll;			// List all records
	bool mListKnownGames;	// Display known games list

	std::string mFilterGame;// Filter records for this game
	std::string mFilterName;// Filter records for this name 

	std::string mImportPath;
	std::string mImportCart;

	sParameters() {
		mAbout = false;
		mHelp = false;
		mList = false;
		mListAll = false;
		mListKnownGames = false;
	}
};
