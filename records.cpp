#include "stdafx.hpp"
#include <chrono>
#include <regex>

static inline std::string ltrim(std::string s, uint8_t pChar) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [pChar](int ch) {
		return ch != pChar;
		}));
	return s;
}
static inline std::string rtrim(std::string s, uint8_t pChar) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [pChar](int ch) {
		return ch != pChar;
		}).base(), s.end());
	return s;
}

cRecords::cRecords() {

	auto json = gResources->FileRead("records.json");

	if(json->size())
		mRecords = Json::parse(json->begin(), json->end());
}

cRecords::~cRecords() {

	gResources->FileSave("records.json", mRecords.dump(1));
}

bool cRecords::add(sRecordRaw* pRawRecords, sKnownGame pGame, size_t pEventID) {
	auto playerName = rtrim(rtrim(pRawRecords[pEventID].getName(), 0x20), 0);
	auto playerScore = ltrim(ltrim(pRawRecords[pEventID].getScore(pGame.mGameID, pEventID), 0x20), 0);

	if (!playerName.size() || !playerScore.size())
		return false;

	if (mRecords.find(pGame.mName) == mRecords.end()) {
		mRecords[pGame.mName] = Json();
	}

	// Look for this record
	for (auto& event_records : mRecords[pGame.mName]) {
		for (auto& event_record : event_records) {
			if (!playerName.compare(event_record["name"]))
				if (!playerScore.compare(event_record["score"]))
					return false;
		}
	}

	auto& eventName = pGame.mEvents[pEventID];
	std::cout << eventName << ": " << playerName << " - " << playerScore << "\n";

	if (mRecords[pGame.mName].find(eventName) == mRecords[pGame.mName].end()) {
		mRecords[pGame.mName][eventName] = Json();
	}

	auto now = std::chrono::system_clock::now();

	auto record = Json();
	record["name"] = playerName;
	record["score"] = playerScore;
	record["gameid"] = pGame.mGameID;
	record["eventid"] = pEventID;
	record["date"] = std::chrono::system_clock::to_time_t(now);

	mRecords[pGame.mName][eventName].emplace_back(record);
	return true;
}

/**
 * Import World Records from the "Epyx Games Collection" cartridge
 *  @see: https://csdb.dk/release/?id=107705
 */
bool cRecords::importCartRecords(const std::string& pFile) {

	auto cart = gResources->FileRead(pFile);
	if (!cart)
		return false;

	uint8_t* raw = cart->data() + 0xBC630;

	for (auto& knowngame : mKnownGames) {

		sRecordRaw* RawRecords = (sRecordRaw*)(raw);
		for (size_t id = 0; id < knowngame.mEvents.size(); ++id)
			gRecords->add(RawRecords, knowngame, id);

		raw += 0x100;
	}

	return true;
}

/**
 * Import records from a disk
 */
bool cRecords::importRecords(const std::string& pFile) {
	cD64 Disk(pFile);

	for (auto& knowngame : mKnownGames) {
		// All disks for this game
		for (auto& knowndisk : knowngame.mDisks) {
			// Label match for this game?
			if (Disk.disklabelGet() == knowndisk.mLabel) {
				auto File = Disk.fileGet(knowndisk.mRecordFile);
				if (File) {
					if (File->mFileSize > 2)
						continue;

					std::cout << knowngame.mName << "\n";

					// + 2 to skip the PRG load address
					sRecordRaw* RawRecords = (sRecordRaw*)(File->mBuffer->data() + 2);
					for (size_t id = 0; id < knowngame.mEvents.size(); ++id)
						gRecords->add(RawRecords, knowngame, id);
					std::cout << "\n";
				}
			}
		}
	}
	return true;
}

/**
 * Find older records on a disk
 */
bool cRecords::findRecords(const std::string& pFile) {
	cD64 Disk(pFile);

	for (auto& knowngame : mKnownGames) {
		// All disks for this game
		for (auto& knowndisk : knowngame.mDisks) {
			// Label match for this game?
			if (Disk.disklabelGet() == knowndisk.mLabel) {

				for (size_t Track = 1; Track <= Disk.trackCount(); ++Track) {
					for (size_t Sector = 0; Sector < Disk.trackRange(Track); ++Sector) {
						auto ptr = Disk.sectorPtr(Track, Sector);
						auto prgLoadAddr = readLEWord(&ptr[0x02]);

						// Highscores load to 0x0E00 and 0x0B00
						if (prgLoadAddr == 0x0E00 || prgLoadAddr == 0x0B00) {
							// +4 skip the T/S chain, and the load address
							sRecordRaw* RawRecords = (sRecordRaw*)(ptr + 4);

							bool ok = false;
							for (int x = 0; x < 0x0a; ++x) {
								uint8_t letter = RawRecords[0].mName[x];
								if (std::isprint(letter))
									ok = true;
								else {
									ok = false;
									break;
								}
							}
							if (ok) {
								for (size_t id = 0; id < knowngame.mEvents.size(); ++id)
									gRecords->add(RawRecords, knowngame, id);
							}
						}
					}
				}
			}
		}
	}
	return true;
}

/**
 * Get all records by 'pName'
 */
tRecords cRecords::getByName(std::string pName) {
	tRecords result;

	// Each game
	for (auto game_records : mRecords) {

		// Each event
		for (auto& event_records : game_records) {

			// Each record
			for (auto& record : event_records) {

				if (!pName.compare(record["name"])) {
					result.push_back({ record["gameid"], record["eventid"], record["name"], record["score"], record["date"] });
				}
			}
		}
	}

	return result;
}

/**
 * Get all records for 'pGame'
 */
tRecordMap cRecords::getByGame(eGames pGame) {
	tRecordMap result;

	auto game = KnownGameByID(pGame);
	// loop each event in this game
	for (auto& event_records : mRecords[game.mName]) {
		// Each record
		for (auto& record : event_records) {
			result.emplace(
				std::make_pair(record["eventid"],
					sRecord{ record["gameid"], record["eventid"], record["name"], record["score"], record["date"] }
			));
		}
	}

	return result;
}
