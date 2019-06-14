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

#include "stdafx.hpp"

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

sKnownGame KnownGameByName(std::string pName) {
	pName = str_to_upper(pName);

	for (auto& game : mKnownGames) {
		if (game.mName == pName)
			return game;
	}

	return {};
}