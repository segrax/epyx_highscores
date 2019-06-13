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
