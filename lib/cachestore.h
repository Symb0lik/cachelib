#pragma once
#include <filesystem>
#include <string>
#include <iostream>
#include <map>

#include "index.h"
#include "datafile.h"
#include "archive.h"
#include "metadata.h"
#include "index_file.h"

class cachestore
{
public:
	cachestore(const std::filesystem::path& cache_dir);
	std::optional<index_file::file_entries> read_index(const unsigned id, const unsigned archive_id);
	std::optional<metadata> get_metadata(const unsigned id);

private:
	const std::string INDEX_FILE_FORMAT = R"(main_file_cache.idx{})";
	const std::string DATA_FILE = R"(main_file_cache.dat2)";
	index index_255;

	std::filesystem::path m_cache_dir;
	std::map<const int, index> m_indexes;

	datafile m_datafile;
};