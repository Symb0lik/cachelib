#pragma once
#include <vector>
#include <optional>
#include <format>
#include <iostream>
#include <memory>
#include <bzlib.h>
#include <zlib.h>

#include "xtea.h"

class archive
{
public:
	enum class compression_type
	{
		NONE,
		BZ2,
		GZ	
	};

	struct archive_metadata
	{
		const compression_type compression;
		const unsigned int compressed_length;
		const unsigned int decompressed_length;
	};

	archive(const std::vector<char>& buffer);
	[[nodiscard("useless if discarded")]] std::optional<archive_metadata> get_metadata() const;
	[[nodiscard("useless call if discarding value")]] std::vector<char> decompress() const;

private:
	char BZIP_HEADER[4] = {'B', 'Z', 'h', '1'};
	const std::vector<char>& m_buffer;

};