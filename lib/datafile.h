#pragma once
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>

#include "index.h"

class datafile
{
public:
	datafile(std::filesystem::path p);
	std::vector<char> read_entry(const index::index_entry& entry);

private:
	const unsigned int SECTOR_SIZE = 520;
	std::filesystem::path m_filename;
	std::ifstream m_filestream;

	[[nodiscard("useless to call and discard value")]] unsigned long long sector_count() const;
};