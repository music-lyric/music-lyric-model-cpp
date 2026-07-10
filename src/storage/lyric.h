#ifndef MUSIC_LYRIC_STORAGE_LYRIC_H_
#define MUSIC_LYRIC_STORAGE_LYRIC_H_

#include <cstdint>
#include <string>
#include <vector>

#include "storage/lyric.pb.h"

namespace music_lyric_model::storage {
	/**
	 * Creates a Lyric stamped with the current SCHEMA_VERSION.
	 */
	lyric::storage::Lyric makeLyric(const lyric::storage::Lyric& init = {});

	/**
	 * Encodes a Lyric into protobuf binary wire format.
	 */
	std::vector<uint8_t> encodeLyric(const lyric::storage::Lyric& model);

	/**
	 * Decodes a Lyric from protobuf binary wire format.
	 * Throws std::runtime_error when the input is not a valid Lyric.
	 */
	lyric::storage::Lyric decodeLyric(const std::vector<uint8_t>& data);

	/**
	 * Encodes a Lyric into its JSON representation.
	 */
	std::string lyricToJson(const lyric::storage::Lyric& model);

	/**
	 * Decodes a Lyric from its JSON representation.
	 * Throws std::runtime_error when the input is not valid Lyric JSON.
	 */
	lyric::storage::Lyric lyricFromJson(const std::string& json);

	/**
	 * Sorts lines and their background lines by start time ascending.
	 */
	void sortLinesByTime(lyric::storage::Lyric& model);
} // namespace music_lyric_model::storage

#endif
