#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include "datafile.h"
#include "archive.h"
#include "metadata.h"
#include "index.h"


int main()
{
	const std::filesystem::path cache_dir = R"(C:\Users\aaron\jagexcache\oldschool\LIVE)";

	for(index idx(255, cache_dir / "main_file_cache.idx255"); const auto &entry: idx.get_entries())
	{
		datafile d(cache_dir / "main_file_cache.dat2");
		auto entry_buf = d.read_entry(entry);
		archive c(entry_buf);

		auto meta = c.get_metadata();

		std::cout << entry.id << std::endl;
		auto data= c.decompress();
		metadata md(data);
		for(const auto& aentry : md.get_entries())
		{
			std::cout << std::format("id: {} named: {} crc: {} revision: {} files: {}\n", aentry.id, aentry.name_hash, aentry.crc, aentry.revision, aentry.file_count);
		}

	}

	return 0;
}