#include "cachestore.h"

#include <ranges>

cachestore::cachestore(const std::filesystem::path& cache_dir) : index_255(255, cache_dir / std::format(INDEX_FILE_FORMAT, 255)), m_cache_dir(cache_dir),
                                                                m_datafile(cache_dir / DATA_FILE)
{
	//for(const auto& entry: index_255.get_entries() | std::views::values)
	//{
	//	m_indexes.emplace(entry.id, index(entry.id, m_cache_dir / std::format(INDEX_FILE_FORMAT, entry.id)));
	//}
	
}

std::optional<index_file::file_entries> cachestore::read_index(const unsigned id, const unsigned archive_id)
{
	if (id > index_255.get_entry_count())
		return std::nullopt;

	auto metadata = get_metadata(id);
	if (!metadata.has_value())
		return std::nullopt;


	index index_entry(id, m_cache_dir / std::format(INDEX_FILE_FORMAT, id));
	const auto archive_entry = index_entry.get_entry(archive_id);
	if (!archive_entry.has_value())
		return std::nullopt;

	const auto archive_data = m_datafile.read_decompressed(archive_entry.value());
	
	const auto metadata_entries = metadata.value().get_entries();
	const auto metadata_entry = metadata_entries.find(static_cast<short>(archive_id));
	if (metadata_entry == metadata_entries.end())
		return std::nullopt;

	index_file fi(archive_data, metadata_entry->second);
	return std::optional<index_file::file_entries>{fi.get_files()};
}

std::optional<metadata> cachestore::get_metadata(const unsigned id)
{
	const auto metadata_entry = index_255.get_entry(id);
	if (!metadata_entry.has_value())
		return std::nullopt;

	const auto entry_data = m_datafile.read_decompressed(metadata_entry.value());

	return std::optional<metadata>{metadata(entry_data)};
}
