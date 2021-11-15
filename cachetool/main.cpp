#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include "index.h"


int main()
{
	const std::filesystem::path cache_dir = R"(C:\Users\aaron\jagexcache\oldschool\LIVE)";
	index idx(255, cache_dir / "main_file_cache.idx255");

	for(const auto & [id, sector, length] : idx.get_entries())
	{
		std::cout <<
			std::format("Entry {} Length: {} Sector: {}\n", id, length, sector);
	}

	return 0;
}