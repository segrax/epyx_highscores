
struct sRecordRaw {
	uint8_t mName[0x0A];
	uint8_t mScore[0x0A];

	std::string getName() const {
		return std::string((const char*)& mName[0], 0x0A);
	}

	std::string getScore(eGames pGame, size_t pEventID) const {

		// Most scores use 7 characters
		std::string res = std::string((const char*)& mScore[0], 0x07);
		switch (pGame) {
		default:
			return res;

		/**
		 * World Games
		 */
		case eGAME_WORLD:
			switch (pEventID) {
			default:
				return res;

			case 6: // Caber
				std::replace(res.begin(), res.end(), (char)0xD6, '\'');
				std::replace(res.begin(), res.end(), (char)0xD5, '"');
				return res;

			case 7: // Sumo
				return std::string((char*)& mScore[0], 0x08);
			}

		/**
		 * Summer Games
		 */
		case eGAME_SUMMER:
			switch (pEventID) {
			default:
				return res;

			case 0: // Pole Vault
				std::replace(res.begin(), res.end(), (char)0xB8, 'm');
				return res;
			}

		/**
		 * Summer Games II
		 */
		case eGAME_SUMMER2:
			switch (pEventID) {
			default:
				return res;

			case 0: // Tripple Jump
			case 4: // High Jump
				std::replace(res.begin(), res.end(), (char)0xB8, 'm');
				return res;
			}


		}
	}
};

struct sRecord {
	eGames mGameID;
	size_t pEventID;
	std::string mName;
	std::string mScore;
	size_t mTimestamp;
};

typedef std::vector<sRecord> tRecords;
typedef std::multimap<size_t, sRecord> tRecordMap;

class cRecords {

public:
	cRecords();
	~cRecords();


	bool add(sRecordRaw* pRawRecords, sKnownGame pGame, size_t pEventID);
	bool importRecords(const std::string& pFile);
	bool findRecords(const std::string& pFile);

	tRecords getByName(std::string pName);
	tRecordMap getByGame(eGames pGame);

protected:
	Json mRecords;


};