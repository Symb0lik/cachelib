#include "index.h"

index::index(int id, std::filesystem::path const &p) : m_filepath_(p)
{
	std::cout << p << std::endl;
	std::cout << std::filesystem::file_size(p);
	m_filestream_ = std::ifstream(p);
}

std::vector<index::index_entry> index::get_entries()
{
	std::vector<index::index_entry> entries;
	for(unsigned int i = 0; i < get_entry_count(); i++)
	{
		if (auto entry = read_entry(i); entry.has_value())
			entries.emplace_back(entry.value());
	}

	return entries;
}

std::optional<index::index_entry> index::read_entry(const unsigned int id)
{
	if (m_filestream_.is_open() && id <= get_entry_count())
		return std::nullopt;

	const auto entry_pos = id * ENTRY_SIZE;
	m_filestream_.seekg(entry_pos);
	if (m_filestream_.tellg() != entry_pos)
		return std::nullopt;

	std::vector<char> buf(ENTRY_SIZE);
	m_filestream_.read(buf.data(), ENTRY_SIZE);

	const unsigned int length = (buf[0] & 0xFF) << 16 | (buf[1] & 0xFF) << 8 | (buf[2] & 0xFF);
	const unsigned int sector = (buf[3] & 0xFF) << 16 | (buf[4] & 0xFF) << 8 | (buf[5] & 0xFF);

	if (length <= 0 || sector <= 0)
		return std::nullopt;

	return std::optional<index::index_entry>{ {id, sector, length} };
}


unsigned long long index::get_entry_count() const
{
	if (!m_filestream_.is_open())
		return 0;

	return std::filesystem::file_size(m_filepath_) / ENTRY_SIZE;
}

