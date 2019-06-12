
#include "stdafx.hpp"
#include <string_view>

std::shared_ptr<cResource> gResources;


enum eGames {
	eGAME_WORLD,
	eGAME_WINTER,
	eGAME_SUMMER,
	eGAME_SUMMER2
};

struct sKnownDisk {
	std::string mLabel;
	std::string mRecordFile;
};

struct sKnownGame {
	eGames mGame;
	std::string mName;

	std::vector<sKnownDisk> mDisks;
	std::vector<std::string> mEvents;
};

struct sRecordRaw {
	uint8_t mName[0x0A];
	uint8_t mScore[0x0A];

	std::string getName() {
		return std::string((char*)&mName[0], 0x0A);
	}

	std::string getScore(eGames pGame, size_t pEventID) {
		std::string res = std::string((char*)& mScore[0], 0x07);
		switch (pGame) {
		default:
			return res;

		case eGAME_WORLD:
			switch (pEventID) {
			default:
				return res;

			case 6: { // Caber
				std::replace(res.begin(), res.end(), (char)0xD6, '\'');
				std::replace(res.begin(), res.end(), (char)0xD5, '"');
				return res;
			}
			case 7: // Sumo
				return std::string((char*)& mScore[0], 0x08);
			}

		case eGAME_SUMMER:
			switch (pEventID) {
			default:
				return res;

			case 0: // Pole Vault
				std::replace(res.begin(), res.end(), (char)0xB8, 'm');
				return res;
		}

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

std::vector<sKnownGame> mKnownGames = {

	// World Games
	{ eGAME_WORLD, "WORLD GAMES", 
		{
			{ "++", "WORLDGAMES3" },
		},
		{
			"Weight Lifting",
			"Barrel Jumping",
			"Cliff Diving",
			"Slalom Skiing",
			"Log Rolling",
			"Bull Riding",
			"Caber Toss",
			"Sumo Wrestling"
		}
	},
	
	// Winter Games
	{ eGAME_WINTER, "WINTER GAMES", 
		{
			{ "WINTER GAMES A", "C" },
			{ "WINTER GAMES B", "C" }
		},
		{
			"Hot Dog",
			"Biathlon",
			"Figure Skating",
			"Ski Jump",
			"Speed Skating",
			"Free Skating",
			"Bobsled"
		}
	},

	// Summer Games
	{ eGAME_SUMMER, "SUMMER GAMES",
		{
			{ "SUMMER GAMES I", "WR" }
		},
		{
			"Pole Vault",
			"Platform Diving",
			"4x400m Relay",
			"100m Dash",
			"Gymnastics",
			"Freestyle Relay",
			"100m Freestyle",
			"Skeet Shooting"
		}
	},

	// Summer Games II
	{ eGAME_SUMMER2, "SUMMER GAMES II", 
		{
			{ "SUMMER GAMES II", "WR" },
			{ "SUMMER GAMES 2B", "WR" }
		},
		{
			"Triple Jump",
			"Rowing",
			"Javelin",
			"Equestrian",
			"High Jump",
			"Fencing",
			"Cycling",
			"Kayaking"
		}
	}
};



Json Records;

int main()
{
	gResources = std::make_shared<cResource>();

	auto files = gResources->directoryList( gResources->getcwd(), "D64");

	for (auto& file : files) {
		bool found = false;
		cD64 Disk(file);

		for (auto& knowngame : mKnownGames) {

			// All disks for this game
			for (auto& knowndisk : knowngame.mDisks) {

				if (Disk.disklabelGet() == knowndisk.mLabel) {

					auto File = Disk.fileGet(knowndisk.mRecordFile);
					if (File) {
						if (File->mFileSize > 2)
							continue;

						found = true;

						std::cout << knowngame.mName << "\n";

						// + 2 to skip the PRG header (load address)
						sRecordRaw* Recordss = (sRecordRaw*) (File->mBuffer->data() + 2);

						for (int id = 0; id < knowngame.mEvents.size(); ++id) {
							auto& res = knowngame.mEvents[id];

							std::cout << res << ": " << Recordss[id].getName() <<  " - " << Recordss[id].getScore(knowngame.mGame, id) << "\n";

						//		Records[knowngame.mName] = Json();
						}

						std::cout << "\n\n";
					}
				}
			}

			if (found)
				break;
		}

	}
}

