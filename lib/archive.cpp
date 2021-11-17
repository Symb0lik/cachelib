#include "archive.h"

archive::archive(const std::vector<char>& buffer) : m_buffer(buffer)
{
}

std::optional<archive::archive_metadata> archive::get_metadata() const
{
	if (m_buffer.empty())
		return std::nullopt;

	compression_type ct;
	switch(char compression_byte = m_buffer[0])
	{
	case 0:
		ct = compression_type::NONE;
		break;
	case 1:
		ct = compression_type::BZ2;
		break;
	case 2:
		ct = compression_type::GZ;
		break;
	default:
		return std::nullopt;
	}

	const unsigned int compressed_size = (m_buffer[1] & 0xFF) << 24
		| (m_buffer[2] & 0xFF) << 16
		| (m_buffer[3] & 0xFF) << 8
		| (m_buffer[4] & 0xFF);

	unsigned int decompressed_size = compressed_size;
	if (ct != compression_type::NONE)
	{
		decompressed_size = (m_buffer[5] & 0xFF) << 24
			| (m_buffer[6] & 0xFF) << 16
			| (m_buffer[7] & 0xFF) << 8
			| (m_buffer[8] & 0xFF);
	} 

	return std::optional<archive::archive_metadata>{ {ct, compressed_size, decompressed_size}};

}

/*
 * This return a newly allocated buffer that holds the decompressed data.
 * Data in the compressed buffer is NOT changed or free'd. 
 */
std::vector<char> archive::decompress() const
{
	const auto check_metadata = get_metadata();
	if (!check_metadata.has_value())
		return {};

	const auto& [compression, compressed_length, decompressed_length] = check_metadata.value();

	std::vector<char> decompressed_buffer(decompressed_length);

	if(compression == compression_type::BZ2)
	{
		const auto compression_stream = std::make_unique<bz_stream>();
		/*
		 * avail_in is compressed size
		 * avail_out is destination decompressed size
		 * next_in is a pointer to the compressed input buffer 
		 * next_out is a pointer to the decompressed output buffer
		 */

		// the osrs archive doesn't include the bzip2 header, so we add it ourselves and initialize with that
		compression_stream->avail_in = sizeof(BZIP_HEADER);
		compression_stream->next_in = const_cast<char*>(BZIP_HEADER);

		compression_stream->avail_out = static_cast<unsigned int>(decompressed_buffer.capacity());
		compression_stream->next_out = decompressed_buffer.data();

		int result = BZ2_bzDecompressInit(compression_stream.get(), 0, 0);
		if (result != BZ_OK)
		{
			return {};
		}

		result = BZ2_bzDecompress(compression_stream.get());
		if(result != BZ_OK)
		{
			BZ2_bzDecompressEnd(compression_stream.get());
			return {};
		}

		// Set the real compressed data. First 9 bytes are the compression metadata, if it is compressed
		compression_stream->avail_in = static_cast<unsigned int>(m_buffer.size() - 9);

		// bzip *should* not modify the input buffer, so this cast is safe to perform
		compression_stream->next_in = const_cast<char*>(m_buffer.data()) + 9;

		result = BZ2_bzDecompress(compression_stream.get());

		BZ2_bzDecompressEnd(compression_stream.get());
		if(result == BZ_OK || result == BZ_STREAM_END)
			return decompressed_buffer;

	} else if (compression == compression_type::GZ)
	{
		const auto compression_stream = std::make_unique<z_stream>();

		// see comments above for descriptions on this. we are skipping the gzip header (+19)
		compression_stream->avail_in = static_cast<unsigned int>(m_buffer.size());
		compression_stream->next_in = reinterpret_cast<unsigned char*>(const_cast<char*>(m_buffer.data())) + 19;
		compression_stream->avail_out = static_cast<unsigned int>(decompressed_buffer.capacity());
		compression_stream->next_out = reinterpret_cast<unsigned char*>(decompressed_buffer.data());

		// -15 = zlib raw deflate 
		int result = inflateInit2(compression_stream.get(), -15);
		if(result != Z_OK)
		{
			return {};
		}

		result = inflate(compression_stream.get(), Z_FINISH);
		inflateEnd(compression_stream.get());

		if (result == Z_OK || result == Z_STREAM_END)
			return decompressed_buffer;
	} else if (compression == compression_type::NONE)
	{
		// Just copy the buffer
		std::copy(m_buffer.begin() + 9, m_buffer.end(), decompressed_buffer.begin());
		return decompressed_buffer;
	}
		
	return {};

}
