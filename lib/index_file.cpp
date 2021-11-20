#include "index_file.h"

index_file::index_file(const std::vector<char>& buf, const metadata::archive_entry& metadata) : m_buf(buf), m_index_metadata(metadata)
{
	//last index is chunk count
	const unsigned chunks = m_buf[m_buf.size() - 1] & 0xFFFF;

	//-1 for chunk count + one int per file slot per chunk
	const auto file_count = metadata.file_count;
	auto read_start = static_cast<unsigned>(m_buf.size() - 1 - (chunks * file_count * 4));

	// will be access by archive = x (chunks * x) + chunk 
	std::vector<unsigned> chunk_sizes(chunks * file_count);
	std::vector<unsigned> sizes(file_count);

	std::cout << std::format("{} chunks\n", chunks);
	for(unsigned chunk = 0; chunk < chunks; chunk++)
	{
		int chunk_size = 0;
		for(unsigned id = 0; id < file_count; id++)
		{
			chunk_size += (m_buf[read_start] & 0xFF) << 24 | (m_buf[read_start + 1] & 0xFF) << 16
				| (m_buf[read_start + 2] & 0xFF) << 8 | (m_buf[read_start + 3] & 0xFF);

			chunk_sizes[id * chunks + chunk] = chunk_size;
			sizes[id] += chunk_size;

			read_start += 4;
		}
	}

	std::vector<std::vector<char>> file_contents(file_count);
	std::vector<unsigned> file_offsets(file_count);

	read_start = 0;
	for(unsigned chunk = 0; chunk < chunks; chunk++)
	{
		for(unsigned id = 0; id < file_count; id++)
		{
			const unsigned chunk_size = chunk_sizes[id * chunks + chunk];
			auto offset = m_buf.begin() + read_start;

			std::vector<char> read_bytes(offset, offset + chunk_size);

			file_contents[id].insert(file_contents[id].end(), read_bytes.begin(), read_bytes.end());

			read_start += chunk_size;
		}
	}

	for(unsigned i = 0; i < file_count; i++)
	{
		auto m = metadata.file_entries[i];
		m_files.emplace(m.id, std::pair(m, file_contents[i]));
	}

}
