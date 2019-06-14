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

struct sRecordRaw {
	uint8_t mName[0x0A];
	uint8_t mScore[0x0A];

	std::string getName() const;
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