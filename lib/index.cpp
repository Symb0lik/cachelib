#include "index.h"

index::index(int id, std::filesystem::path p) : m_filepath(p)
{
	m_filestream = std::ifstream(p, std::ios_base::in | std::ios_base::binary);
}

std::optional<index::index_entry> index::get_entry(const unsigned id)
{
	if (id > get_entry_count())
	{
		return std::nullopt;
	}

	if (const auto entry = m_index_entries.find(id); entry != m_index_entries.end())
		return entry->second;

	return read_entry(id);
}

std::optional<index::index_entry> index::read_entry(const unsigned int id)
{
	if (m_filestream.is_open() && id >= get_entry_count())
		return std::nullopt;

	const auto entry_pos = id * ENTRY_SIZE;
	m_filestream.seekg(entry_pos);
	if (m_filestream.tellg() != entry_pos)
		return std::nullopt;

	std::vector<char> buf(ENTRY_SIZE);
	m_filestream.read(buf.data(), ENTRY_SIZE);

	const unsigned int length = (buf[0] & 0xFF) << 16 | (buf[1] & 0xFF) << 8 | (buf[2] & 0xFF);
	const unsigned int sector = (buf[3] & 0xFF) << 16 | (buf[4] & 0xFF) << 8 | (buf[5] & 0xFF);

	if (length <= 0 || sector <= 0)
		return std::nullopt;

	index_entry entry(id, sector, length);
	m_index_entries.emplace(id, entry);
	return std::optional<index_entry>{entry};
}


unsigned long long index::get_entry_count() const
{
	if (!m_filestream.is_open())
		return 0;

	return file_size(m_filepath) / ENTRY_SIZE;
}
