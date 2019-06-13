
#include "stdafx.hpp"

std::shared_ptr<cResource> gResources;
std::shared_ptr<cRecords> gRecords;

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

sKnownGame KnownGameByID(eGames pGame) {

	for (auto& game : mKnownGames) {
		if (game.mGameID == pGame)
			return game;
	}

	return {};
}

int main()
{
	gResources = std::make_shared<cResource>();
	gRecords = std::make_shared<cRecords>();

	std::cout << "Searching for D64 files in current directory\n";
	auto files = gResources->directoryList( gResources->getcwd(), "D64");
	for (auto& file : files) {

		gRecords->importRecords(file);
		gRecords->findRecords(file);
	}

	std::cout << "\nListing Records\n";
	//gRecords->getByName("ROBERT");
	for (auto& game : mKnownGames) {
		auto records = gRecords->getByGame(game.mGameID);

		std::cout << "\n" << game.mName << "\n";
		for (auto& record : records) {
			std::cout << game.mEvents[record.first] << ": ";
			std::cout << record.second.mName << " - " << record.second.mScore << "\n";

		}
	}
}

