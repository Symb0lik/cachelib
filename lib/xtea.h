#pragma once
#include <vector>
class xtea
{
public:
	static std::vector<char> decrypt(const std::vector<char>& buf);

private:
	static constexpr unsigned long long GOLDEN_RATIO = 0x9E3779B9;
	static constexpr unsigned long long ROUNDS = 32;
};