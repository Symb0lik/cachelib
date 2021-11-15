#pragma once

#include <filesystem>
#include <optional>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

class index
{
	struct index_entry
	{
		const unsigned int id;
		const unsigned int sector;
		const unsigned int length;
	};

public:
	index(int id, const std::filesystem::path& p);
	std::vector<index_entry> get_entries();
	std::optional<index_entry> read_entry(const unsigned int id);
	[[nodiscard("Entry count is useless if discarded")]] unsigned long long get_entry_count() const;

private:
	const int ENTRY_SIZE = 6;

	std::filesystem::path const &m_filepath_;
	std::ifstream m_filestream_;


};