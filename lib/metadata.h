#pragma once

#include <vector>
#include <iostream>
#include <format>
#include <map>
class metadata
{
public:
	struct archive_file_entry
	{
		unsigned int id;
		unsigned int name_hash;
	};

	struct archive_entry
	{
		const unsigned short id;
		const unsigned int name_hash;
		const unsigned int crc;
		const unsigned int revision;
		const unsigned short file_count;
		std::vector<archive_file_entry> file_entries;
	};

	metadata(const std::vector<char>& buf);
	std::map<const short, archive_entry> get_entries() const;
private:
	const std::vector<char>& m_buf;
	bool m_named = false;
	int m_read = 0;
	unsigned int m_protocol = 0;
	unsigned int m_revision = 0;
	unsigned int m_archives_count = 0;
	std::map<const short, archive_entry> m_entries;

	void read_entries();

	unsigned int read_int32();
	unsigned short read_short16();
	unsigned char read_byte8();


};