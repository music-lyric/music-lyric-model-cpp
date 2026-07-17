#ifndef MUSIC_LYRIC_STORAGE_LYRIC_H_
#define MUSIC_LYRIC_STORAGE_LYRIC_H_

#include <cstdint>
#include <string>
#include <vector>

#include "storage/lyric.pb.h"

namespace music_lyric_model::storage {
	/**
	 * Creates a Lyric, stamping the current SCHEMA_VERSION.
	 */
	lyric::storage::Lyric makeStorageLyric(const lyric::storage::Lyric& init = {});

	/**
	 * Encode a Lyric to protobuf binary.
	 * Throws std::runtime_error when encoding fails.
	 */
	std::vector<uint8_t> encodeStorageLyric(const lyric::storage::Lyric& lyric);

	/**
	 * Decode a Lyric from protobuf binary.
	 * Throws std::runtime_error when the input is not a valid Lyric.
	 */
	lyric::storage::Lyric decodeStorageLyric(const std::vector<uint8_t>& data);

	/**
	 * Convert a Lyric to protobuf JSON.
	 * Throws std::runtime_error when encoding fails.
	 */
	std::string storageLyricToJson(const lyric::storage::Lyric& lyric);

	/**
	 * Restore a Lyric from protobuf JSON.
	 * Throws std::runtime_error when the input is not valid Lyric JSON.
	 */
	lyric::storage::Lyric storageLyricFromJson(const std::string& json);

	/**
	 * Sort lines and their background lines by start time ascending.
	 */
	void sortStorageLinesByTime(lyric::storage::Lyric& lyric);
} // namespace music_lyric_model::storage

#endif
