#include "stdafx.hpp"
#include <sstream>
#include <fstream>

tSharedBuffer cResource::FileRead(const std::string& pFile) {
	std::ifstream* fileStream;
	auto			fileBuffer = std::make_shared<std::vector<uint8_t>>();

	// Attempt to open the file
	fileStream = new std::ifstream(pFile.c_str(), std::ios::binary);
	if (fileStream->is_open() != false) {

		// Get file size
		fileStream->seekg(0, std::ios::end);
		fileBuffer->resize(static_cast<const unsigned int>(fileStream->tellg()));
		fileStream->seekg(std::ios::beg);

		// Allocate buffer, and read the file into it
		fileStream->read((char*)fileBuffer->data(), fileBuffer->size());
		if (!(*fileStream))
			fileBuffer->clear();
	}

	// Close the stream
	fileStream->close();
	delete fileStream;

	// All done ;)
	return fileBuffer;
}

bool cResource::FileSave(const std::string& pFile, const std::string& pData) {

	std::ofstream outfile(pFile, std::ofstream::binary);
	if (!outfile.is_open())
		return false;

	outfile << pData;
	outfile.close();
	return true;
}

bool cResource::FileExists(const std::string& pPath) const {
	struct stat info;

	if (stat(pPath.c_str(), &info) != 0)
		return false;
	else if (info.st_mode & S_IFDIR)
		return true;
	else if (info.st_mode & S_IFMT)
		return true;

	return false;
}

bool cResource::isFile(const std::string& pPath) const {
	struct stat info;

	if (stat(pPath.c_str(), &info) != 0)
		return false;
	else if (info.st_mode & S_IFDIR)
		return false;
	else if (info.st_mode & S_IFMT)
		return true;

	return false;
}
#ifdef WIN32
#include "Windows.h"
#include <direct.h>

std::string cResource::getcwd() {
	char buff[1024];
	_getcwd(buff, 1024);
	std::string cwd(buff);
	return cwd;
}

std::vector<std::string> cResource::directoryList(const std::string& pPath, const std::string& pExtension) {
	WIN32_FIND_DATA fdata;
	HANDLE dhandle;
	std::vector<std::string> results;

	// Build the file path
	std::stringstream finalPath;

	if (pPath.size())
		finalPath << pPath;

	finalPath << "/*" << pExtension;

	size_t size = MultiByteToWideChar(0, 0, finalPath.str().c_str(), (int)finalPath.str().length(), 0, 0);
	WCHAR* pathFin = new WCHAR[size + 1];
	memset(pathFin, 0, size + 1);

	size = MultiByteToWideChar(0, 0, finalPath.str().c_str(), (int)size, pathFin, (int)size);
	pathFin[size] = 0;

	if ((dhandle = FindFirstFile(pathFin, &fdata)) == INVALID_HANDLE_VALUE) {
		delete pathFin;
		return results;
	}

	delete pathFin;
	size_t tmp = 0;

	{
		char* file = new char[wcslen(fdata.cFileName) + 1];
		memset(file, 0, wcslen(fdata.cFileName) + 1);

		wcstombs_s(&tmp, file, wcslen(fdata.cFileName) + 1, fdata.cFileName, wcslen(fdata.cFileName));
		results.push_back(std::string(file));
		delete file;
	}

	while (1) {
		if (FindNextFile(dhandle, &fdata)) {
			char* file = new char[wcslen(fdata.cFileName) + 1];
			memset(file, 0, wcslen(fdata.cFileName) + 1);

			wcstombs_s(&tmp, file, wcslen(fdata.cFileName) + 1, fdata.cFileName, wcslen(fdata.cFileName));
			results.push_back(std::string(file));
			delete file;

		}
		else {
			if (GetLastError() == ERROR_NO_MORE_FILES) {
				break;
			}
			else {
				FindClose(dhandle);
				return results;
			}
		}
	}

	FindClose(dhandle);

	return results;
}

#else
#include <dirent.h>
std::string findType;

std::string cResource::getcwd() {
	char buff[1024];
	::getcwd(buff, 1024);
	std::string cwd(buff);
	return cwd;
}

int file_select(const struct dirent* entry) {
	std::string name = entry->d_name;

	transform(name.begin(), name.end(), name.begin(), ::toupper);

	if (name.find(findType) == std::string::npos)
		return false;

	return true;
}

std::vector<std::string> cResource::DirectoryList(const std::string& pPath, const std::string& pExtension) {
	struct dirent** directFiles;
	std::vector<std::string>		  results;

	// Build the file path
	std::stringstream finalPath;

	finalPath << pPath << "/";

	findType = pExtension;

	transform(findType.begin(), findType.end(), findType.begin(), ::toupper);

	int count = scandir(finalPath.str().c_str(), (dirent * **)& directFiles, file_select, 0);

	for (int i = 0; i < count; ++i) {

		results.push_back(std::string(directFiles[i]->d_name));
	}

	transform(findType.begin(), findType.end(), findType.begin(), ::tolower);

	count = scandir(finalPath.str().c_str(), (dirent * **)& directFiles, file_select, 0);

	for (int i = 0; i < count; ++i) {

		results.push_back(std::string(directFiles[i]->d_name));
	}

	return results;
}

#endif
