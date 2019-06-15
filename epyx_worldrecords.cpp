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
#include "cxxopts.hpp"
#include <iomanip>

std::shared_ptr<cResource> gResources;
std::shared_ptr<cRecords> gRecords;
std::shared_ptr<cxxopts::Options> gCliOptions;
sParameters gParameters;

void cli_prepare() {

	gCliOptions = std::make_shared<cxxopts::Options>("epyx_worldrecords", "Epyx World Records: Keep your world records");
	gCliOptions->allow_unrecognised_options();
	gCliOptions->add_options()
		("about", "About", cxxopts::value<bool>()->default_value("false"))
		("h,help", "Help", cxxopts::value<bool>()->default_value("false"))

		("games", "List known games", cxxopts::value<bool>()->default_value("false"))

		("filter-game", "Filter records for game", cxxopts::value<std::string>()->default_value(""), "\"World Games\"")
		("filter-name", "Filter records for name", cxxopts::value<std::string>()->default_value(""), "\"My Name\"")
		("filter-date-before", "Filter records occurring before date", cxxopts::value<std::string>()->default_value(""), "\"2019-06-15\"")
		("filter-date-after", "Filter records occurring after date", cxxopts::value<std::string>()->default_value(""), "\"2019-06-15\"")

		("list-all", "List records", cxxopts::value<bool>()->default_value("false"))

		("import-path", "Search Path", cxxopts::value<std::string>()->default_value(""), "\"Path\"")
		("import-cart", "Import from the Epyx cartridge", cxxopts::value<std::string>()->default_value(""), "\"Path.crt\"");
}

void cli_process(int argc, char* argv[]) {
	auto result = gCliOptions->parse(argc, argv);

	if (result["help"].as<bool>() == true) {
		gParameters.mHelp = true;
		return;
	}

	if (result["about"].as<bool>() == true) {
		gParameters.mAbout = true;
	}

	if (result["list-all"].as<bool>() == true) {
		gParameters.mList = true;
		gParameters.mListAll = true;
	}
	if (result["games"].as<bool>() == true) {
		gParameters.mListKnownGames = true;
	}

	std::string filterdate = result["filter-date-after"].as<std::string>();
	if (filterdate.size()) {
		gParameters.mFilterDateForward = true;
	} else {
		filterdate = result["filter-date-before"].as<std::string>();
		gParameters.mFilterDateForward = false;
	}

	
	if (filterdate.size()) {
		std::tm tm = { 0 };
		std::istringstream ss(filterdate);
		ss >> std::get_time(&tm, "%Y-%m-%d");
		gParameters.mFilterDate = std::mktime(&tm);
	}

	gParameters.mFilterGame = result["filter-game"].as<std::string>();
	gParameters.mFilterName = result["filter-name"].as<std::string>();

	gParameters.mImportPath = result["import-path"].as<std::string>();
	gParameters.mImportCart = result["import-cart"].as<std::string>();
}

int main(int argc, char* argv[])
{
	cli_prepare();
	cli_process(argc, argv);

	gResources = std::make_shared<cResource>();
	gRecords = std::make_shared<cRecords>();

	// Show the help
	if (gParameters.mHelp) {
		std::cout << gCliOptions->help() << "\n";
		return 0;
	}

	if (gParameters.mAbout) {

		return 0;
	}

	// List all known games
	if (gParameters.mListKnownGames) {
		std::cout << "Known Games\n\n";
		for (auto& knowngame : mKnownGames) {
			std::cout << "* " << knowngame.mName << "\n";
		}
		return 0;
	}


	// Import a specific cartridge?
	if (gResources->isFile(gParameters.mImportCart)) {
		return gRecords->importCartRecords(gParameters.mImportCart) ? 0 : 1;

	} else if (gParameters.mImportCart.size()) {
		std::cout << "File not found: " << gParameters.mImportCart;
		return 1;
	}

	// Import D64/CRT files from a path, or current directory
	if (gParameters.mImportPath.size() || !gParameters.mList) {
		
		if(!gParameters.mImportPath.size())
			gParameters.mImportPath = gResources->getcwd();

		std::cout << "Importing from path: " << gParameters.mImportPath << "\n";

		std::cout << "Searching for D64 files\n";
		auto files = gResources->directoryList(gParameters.mImportPath, "D64");
		for (auto& file : files) {

			std::cout << " " << file << "\n";
			gRecords->importRecordsDisk(file);
			gRecords->findRecordsDisk(file);
		}

		std::cout << "Searching for CRT files\n";
		files = gResources->directoryList(gParameters.mImportPath, "crt");
		for (auto& file : files) {
			std::cout << " " << file << "\n";
			gRecords->importCartRecords(file);
		}

		// Unless specically told to list, exit after importing
		if(!gParameters.mList)
			return 0;
	}

	// Listing records?
	if (gParameters.mList) {

		std::cout << "\nListing Records\n";

		// Filter by game
		if (gParameters.mFilterGame.size()) {
			auto filtergame = KnownGameByName(gParameters.mFilterGame);
			std::cout << gRecords->dumpRecordsByGame(filtergame.mGameID);
			return 0;
		}

		std::cout << gRecords->dumpRecordsAll();

		return 0;
	}

	std::cout << gCliOptions->help() << "\n";
	return 1;
}
