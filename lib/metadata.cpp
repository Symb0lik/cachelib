#include "metadata.h"

metadata::metadata(const std::vector<char>& buf) : m_buf(buf), m_read(0)
{
	m_protocol = (0xFFFF & read_byte8());
	if(m_protocol >= 6)
	{
		m_revision = read_int32();
	}
	m_named = (1 & read_byte8()) != 0;
	m_archives_count = read_short16();
	read_entries();
}

std::map<const short, metadata::archive_entry> metadata::get_entries() const
{
	return m_entries;
}

void metadata::read_entries()
{
	struct temp_entry
	{
		unsigned short id = 0;
		unsigned int named_hash = 0;
		unsigned int crc = 0;
		unsigned int revision = 0;
		unsigned short file_count = 0;
		std::vector<archive_file_entry> file_entries;
	};

	std::vector<temp_entry> temp_entries(m_archives_count);

	/*
	 * The following could be done in a single loop, but doing it this way matches the
	 * way the file is laid out on disk more closely. 
	 */

	unsigned short last_archive_id = 0;
	for(unsigned i = 0; i < m_archives_count; i++)
	{
		const unsigned short id = last_archive_id += read_short16();
		temp_entries[i].id = id;
	}

	if (m_named)
	{
		for (unsigned i = 0; i < m_archives_count; i++)
		{
			const unsigned int named_hash = read_int32();
			temp_entries[i].named_hash = named_hash;
		}
	}

	for(unsigned i = 0; i < m_archives_count; i++)
	{
		const unsigned int crc = read_int32();
		temp_entries[i].crc = crc;
	}

	for(unsigned i = 0; i < m_archives_count; i++)
	{
		const unsigned int revision = read_int32();
		temp_entries[i].revision = revision;
	}

	for(unsigned i = 0; i < m_archives_count; i++)
	{
		const unsigned short file_count = read_short16();
		temp_entries[i].file_count = file_count;
		temp_entries[i].file_entries.resize(file_count);
	}

	for(unsigned i = 0; i < m_archives_count; i++)
	{
		int last = 0;
		for (unsigned j = 0; j < temp_entries[i].file_count; j++)
		{
			const unsigned int file_id = last += read_short16();
			temp_entries[i].file_entries[j].id = file_id;
		}
	}

	if(m_named)
	{
		for(unsigned i = 0; i < m_archives_count; i++)
		{
			for(unsigned j = 0; j < temp_entries[i].file_count; j++)
			{
				temp_entries[i].file_entries[j].name_hash = read_int32();
			}
		}
	}

	for (const auto& entry : temp_entries)
	{
		archive_entry new_entry = {entry.id, entry.named_hash, entry.crc, entry.revision, entry.file_count, entry.file_entries};
		m_entries.emplace(entry.id, new_entry);
	}
}

unsigned metadata::read_int32()
{
	const unsigned result = (m_buf[m_read] & 0xFF) << 24 | (m_buf[m_read + 1] & 0xFF) << 16 | (m_buf[m_read + 2] & 0xFF) << 8
		| (m_buf[m_read + 3] & 0xFF);

	m_read += 4;
	return result;
}

unsigned short metadata::read_short16()
{
	const unsigned short result = (m_buf[m_read] & 0xFF) << 8 | (m_buf[m_read + 1] & 0xFF);

	m_read += 2;
	return result;
}

unsigned char metadata::read_byte8()
{
	return m_buf[m_read++] & 0xFF;
}
