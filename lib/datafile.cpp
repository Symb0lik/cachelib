#include "datafile.h"

datafile::datafile(std::filesystem::path p) : m_filename(p)
{
	m_filestream = std::ifstream(p, std::ios_base::in | std::ios_base::binary);
}

std::vector<char> datafile::read_entry(const index::index_entry& entry)
{
	if (!m_filestream.is_open() || sector_count() < entry.sector)
		return {};

	std::vector<char> entry_buffer(entry.length);
	std::vector<char> read_buffer(SECTOR_SIZE);

	int next_sector = 0;
	unsigned int current_sector = entry.sector;
	unsigned int read_bytes = 0;
	int part = 0;

	/*
	 * Read sector by sector until we've read all the index's bytes
	 * A sector is formatted like:
	 * { 
	 *	 header: 8 or 10 bytes depending on archive_id
	 *	 data: 510 or 512 bytes depending on header size
	 * } == 520 bytes
	 */

	do
	{
		if (entry.sector == 0)
			break;

		const auto sector_pos = current_sector * SECTOR_SIZE;
		m_filestream.seekg(sector_pos);
		if (m_filestream.tellg() != sector_pos)
			break;

		const auto remaining_data = entry.length - read_bytes;
		auto next_read_size = remaining_data > SECTOR_SIZE ? SECTOR_SIZE : remaining_data;
		uint8_t header_size = 0;
		int current_index = 0;
		int current_part = 0;
		int current_archive = 0;

		/*
		 * Sector Header
		 * size: 10 if archive_id > 0xFFFFF
		 *   current_archive: 4 bytes
		 *   current_part: 2 bytes
		 *   next_sector: 3 bytes
		 *	 index: 1 byte
		 *
		 * size: 8 if archive_id <= 0xFFFF
		 *   current_archive: 2 bytes
		 *   current_part: 2 bytes
		 *	 next_sector: 3 bytes
		 *	 index: 1 byte
		 */
	
		if(entry.id > 0xFFFF)
		{
			header_size = 10;

			if (next_read_size + header_size < SECTOR_SIZE)
				next_read_size += header_size;

			m_filestream.read(read_buffer.data(), next_read_size);
			if (m_filestream.gcount() != next_read_size)
				break;

			current_archive = (read_buffer[0] & 0xFF) << 24
				| (read_buffer[1] & 0xFF) << 16
				| (read_buffer[2] & 0xFF) << 8
				| (read_buffer[3] & 0xFF);

			current_part = ((read_buffer[4] & 0xFF) << 8) + (read_buffer[5] & 0xFF);

			next_sector = (read_buffer[6] & 0xFF) << 16
				| (read_buffer[7] & 0xFF) << 8
				| (read_buffer[8] & 0xFF);

			current_index = read_buffer[9] & 0xFF;

		} else
		{
			header_size = 8;

			if (next_read_size + header_size < SECTOR_SIZE)
				next_read_size += header_size;

			m_filestream.read(read_buffer.data(), next_read_size);
			if (m_filestream.gcount() != next_read_size)
				break;


			current_archive = (read_buffer[0] & 0xFF) << 8
				| (read_buffer[1] & 0xFF);

			current_part = (read_buffer[2] & 0xFF) << 8
				| (read_buffer[3] & 0xFF);

			next_sector = (read_buffer[4] & 0xFF) << 16
				| (read_buffer[5] & 0xFF) << 8
				| (read_buffer[6] & 0xFF);

			current_index = read_buffer[7] & 0xFF;
		}

		// we've already pre-allocated space in our vector to hold all the data, we only need to copy it in
		std::copy(read_buffer.begin() + header_size, read_buffer.begin() + next_read_size, entry_buffer.begin() + read_bytes);
		read_bytes += next_read_size - header_size;
		current_sector = next_sector;

		part++;
		
	} while (read_bytes < entry.length);

	return entry_buffer;
}

unsigned long long datafile::sector_count() const
{
	return std::filesystem::file_size(m_filename) / SECTOR_SIZE;
}
