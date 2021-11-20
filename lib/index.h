#pragma once

#include <filesystem>
#include <optional>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <map>

class index
{
public:
	struct index_entry
	{
		const unsigned int id;
		const unsigned int sector;
		const unsigned int length;
	};

	index(int id, std::filesystem::path p);
	[[nodiscard("Entries must be consumed")]] const std::map<const unsigned, index_entry>&get_entries() { return m_index_entries; }
	std::optional<index_entry> get_entry(const unsigned id);
	std::optional<index_entry> read_entry(const unsigned int id);
	[[nodiscard("Entry count is useless if discarded")]] unsigned long long get_entry_count() const;

private:
	const int ENTRY_SIZE = 6;

	std::filesystem::path m_filepath;
	std::ifstream m_filestream;
	std::map<const unsigned, index_entry> m_index_entries;

	void load_entries();

};