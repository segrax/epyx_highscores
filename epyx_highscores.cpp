
#include "stdafx.hpp"

std::shared_ptr<cResource> gResources;
std::shared_ptr<cRecords> gRecords;

std::vector<sKnownGame> mKnownGames = {

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
		},
		{
			1,	// Pole
			1,	// Platform
			0,	// 4x400m
			0,	// 100m Dash
			1,	// Gym
			0,	// Freestyle
			0,	// 100m Free
			1	// Skeet
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
		},
		{
			1,	// Triple Jump
			0,	// Rowing
			1,	// Javelin
			0,	// Equest
			1,	// High
			0,	// Fencing
			1,	// Cycl
			1	// Kayaking
		}
	},

	// Winter Games
	{ eGAME_WINTER, "WINTER GAMES",
		{
			{ "WINTER GAMES A", "C" },
			{ "WINTER GAMES B", "C" },
			{ "WINER GAMES B", "C" }
		},
		{
			"Hot Dog",
			"Biathlon",
			"Figure Skating",
			"Ski Jump",
			"Speed Skating",
			"Free Skating",
			"Bobsled"
		},
		{
			1,	// Hot Dog
			0,	// Biathlon
			1,	// Figure
			1,	// Ski Jump
			0,	// Speed Skate
			1,	// Free Skate
			1	// Bobsled
		}
	},

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
		},
		{
			1,	// Weight
			1,	// Barrel
			1,	// Cliff
			0,	// Slalom
			1,	// Log
			1,	// Bull
			1,	// Caber
			0	// Sumo
		}
	},

	// California Games
	{ eGAME_CALIFORNIA, "CALIFORNIA GAMES",
		{
			{ "EAGLE SOFT INC.", "HS"}
		},
		{
			"Halfpipe",
			"Footbag",
			"Surfing",
			"Skating",
			"BMX",
			"Flying disk"
		},
		{
			0,
			0,
			0,
			0,
			0,
			0
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

		gRecords->importRecordsDisk(file);
		gRecords->findRecordsDisk(file);
	}

	std::cout << "Searching for CRT files in current directory\n";
	files = gResources->directoryList(gResources->getcwd(), "crt");
	for (auto& file : files) {
		gRecords->importCartRecords(file);
	}

	std::cout << "\nListing Records\n";
	std::cout << gRecords->dumpAllRecords();

	//gRecords->getByName("ROBERT");
}

