
struct sRecordRaw {
	uint8_t mName[0x0A];
	uint8_t mScore[0x0A];

	std::string getName() const {
		return std::string((const char*)& mName[0], 0x0A);
	}
	std::string getScore(eGames pGame, size_t mEventID) const;
};

struct sRecord {
	eGames mGameID;
	size_t mEventID;
	std::string mName;
	std::string mScore;
	size_t mTimestamp;

	bool operator<(const sRecord& pRight) const;
};

typedef std::vector<sRecord> tRecords;
typedef std::map<size_t, tRecords> tRecordMap;	// EventID, Records

class cRecords {

public:
	cRecords();
	~cRecords();

	bool add(sRecordRaw* pRawRecords, sKnownGame pGame, size_t mEventID);
	bool importCartRecords(const std::string& pFile);
	bool importRecordsDisk(const std::string& pFile);
	bool findRecordsDisk(const std::string& pFile);

	tRecords getByName(std::string pName);
	tRecordMap getByGame(eGames pGame);

	std::string dumpRecordsForEvent(const eGames pGame, const size_t pEventID, const tRecords& pRecord);
	std::string dumpRecordsByGame(eGames pGame);
	std::string dumpRecordsAll();

protected:
	Json mRecords;

private:
	bool mHasChanged;
};