#include "xtea.h"

std::vector<char> xtea::decrypt(const std::vector<char>& buf)
{
	const auto num_blocks = static_cast<unsigned int>(buf.size() / 8);
	auto pos = 0;

	std::vector<char> result_buffer;
	for(unsigned int block = 0; block < num_blocks; block++)
	{
		char v0_buf[4] = {buf[0], buf[1], buf[2], buf[3]};
		char v1_buf[4] = {buf[4], buf[5], buf[6], buf[7]};

		unsigned int v0 = *reinterpret_cast<unsigned int*>(v0_buf);
		unsigned int v1 = *reinterpret_cast<unsigned int*>(v1_buf);

		unsigned long long sum = GOLDEN_RATIO * ROUNDS;

		for(unsigned int i = 0; i < ROUNDS; i++)
		{
			v1 -= ((v0 << 4 ^ (v0 >> 5)) + v0) ^ (sum + 0);
			sum -= GOLDEN_RATIO;
			v0 -= ((v1 << 4) ^ (v1 >> 5)) + v1 ^ (sum);
		}

		for(unsigned int i = 0; i <  sizeof(v0); i++)
		{
			const auto intptr = reinterpret_cast<char*>(&v0);
			result_buffer.push_back(intptr[i]);
		}

		for(unsigned int i = 0; i <  sizeof(v1); i++)
		{
			const auto intptr = reinterpret_cast<char*>(&v1);
			result_buffer.push_back(intptr[i]);
		}

	}

	return result_buffer;

}
