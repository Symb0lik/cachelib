#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include "datafile.h"
#include "archive.h"
#include "metadata.h"
#include "index.h"
#include "cachestore.h"
#include "index_file.h"


int main()
{
	const std::filesystem::path cache_dir = R"(C:\Users\aaron\jagexcache\oldschool\LIVE)";

	cachestore c(cache_dir);
	auto files = c.read_index(2, 10);

	for(const auto& file : files.value()) 
	{
		auto id = file.first;
		auto metadata = file.second.first;
		const auto& data = file.second.second;

		std::cout << std::format("{} : {}\n\t", id, metadata.id);
		for (const auto& d : data)
		{
			std::cout << d;
		}

		std::cout << std::endl;
	}
	return 0;
}