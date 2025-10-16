#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <random>

namespace EncryptionUtils
{
inline std::array<uint8_t, 256> GenerateEncryptionTable(uint32_t key)
{
	std::array<uint8_t, 256> table{};

	for (int i = 0; i < 256; ++i)
	{
		table[i] = static_cast<uint8_t>(i);
	}

	std::mt19937 generator(key);
	std::ranges::shuffle(table, generator);

	return table;
}

inline std::array<uint8_t, 256> GenerateDecryptionTable(uint32_t key)
{
	auto encryptionTable = GenerateEncryptionTable(key);
	std::array<uint8_t, 256> decryptionTable{};

	for (int i = 0; i < 256; ++i)
	{
		decryptionTable[encryptionTable[i]] = static_cast<uint8_t>(i);
	}

	return decryptionTable;
}
} // namespace EncryptionUtils