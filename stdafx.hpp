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

#include <iostream>
#include <vector>
#include <memory>

typedef std::shared_ptr<std::vector<uint8_t>> tSharedBuffer;

#include "json.hpp"
using Json = nlohmann::json;

#include "d64.hpp"
#include "resource.hpp"
#include "epyx_worldrecords.hpp"
#include "records.hpp"

extern std::vector<sKnownGame> mKnownGames;
extern std::shared_ptr<cResource> gResources;
extern std::shared_ptr<cRecords> gRecords;
extern sParameters gParameters;

sKnownGame KnownGameByID(eGames pGame);
sKnownGame KnownGameByName(std::string pName);

// Read a word from the buffer
inline uint16_t readWord(const void* buffer) {
	const uint16_t* wordBytes = (const uint16_t*)buffer;
	return *wordBytes;
}

// Read a word from the buffer
inline uint16_t readLEWord(const void* buffer) {
	const uint16_t* wordBytes = (const uint16_t*)buffer;
	return *wordBytes;
}

inline void writeLEWord(const void* buffer, uint16_t pValue) {
	uint16_t* wordBytes = (uint16_t*)buffer;
	*wordBytes = pValue;
}

inline std::string str_to_upper(std::string pStr) {
	std::transform(pStr.begin(), pStr.end(), pStr.begin(), ::toupper);
	return pStr;
}

