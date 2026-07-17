#ifndef MUSIC_LYRIC_MODEL_STORAGE_LYRIC_H_
#define MUSIC_LYRIC_MODEL_STORAGE_LYRIC_H_

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/agent.h"
#include "common/common.h"
#include "common/meta.h"
#include "line.h"

namespace music_lyric_model::storage {
	/**
	 * A storage lyric document.
	 */
	struct Lyric {
		std::string                                  version;
		common::Timing                               timing = common::Timing::Unspecified;
		std::unordered_map<std::string, std::string> extra;
		common::Meta                                 meta;
		std::vector<common::AgentItem>               agents;
		std::vector<Line>                            lines;
	};

	/**
	 * Creates a Lyric, stamping the current SCHEMA_VERSION.
	 */
	Lyric makeStorageLyric(Lyric init = {});

	/**
	 * Encode a Lyric to protobuf binary.
	 * Throws std::runtime_error when encoding fails.
	 */
	std::vector<uint8_t> encodeStorageLyric(const Lyric& lyric);

	/**
	 * Decode a Lyric from protobuf binary.
	 * Throws std::runtime_error when the input is not a valid Lyric.
	 */
	Lyric decodeStorageLyric(const std::vector<uint8_t>& data);

	/**
	 * Convert a Lyric to protobuf JSON.
	 * Throws std::runtime_error when encoding fails.
	 */
	std::string storageLyricToJson(const Lyric& lyric);

	/**
	 * Restore a Lyric from protobuf JSON.
	 * Throws std::runtime_error when the input is not valid Lyric JSON.
	 */
	Lyric storageLyricFromJson(const std::string& json);

	/**
	 * Sort lines and their background lines by start time ascending.
	 */
	void sortStorageLinesByTime(Lyric& lyric);
} // namespace music_lyric_model::storage

#endif
