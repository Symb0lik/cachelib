#pragma once
#include <vector>
#include <map>
#include <iostream>

#include "metadata.h"

class index_file
{
public:
	index_file(const std::vector<char>& buf, const metadata::archive_entry& metadata);
	auto get_files() { return m_files; }

	using file_entries = std::map<unsigned, std::pair<metadata::archive_file_entry, std::vector<char>>>;

private:
	const std::vector<char>& m_buf;
	metadata::archive_entry m_index_metadata;

	 file_entries m_files;
};