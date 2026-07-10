#ifndef MUSIC_LYRIC_RUNTIME_INFO_H_
#define MUSIC_LYRIC_RUNTIME_INFO_H_

#include <cstdint>
#include <string>
#include <vector>

#include "runtime/info.pb.h"

namespace music_lyric_model {
	using namespace lyric::runtime;

	/**
	 * Creates an Info stamped with the current SCHEMA_VERSION.
	 */
	lyric::runtime::Info makeInfo(const lyric::runtime::Info& init = {});

	/**
	 * Encodes an Info into protobuf binary wire format.
	 */
	std::vector<uint8_t> encodeInfo(const lyric::runtime::Info& info);

	/**
	 * Decodes an Info from protobuf binary wire format.
	 * Throws std::runtime_error when the input is not a valid Info.
	 */
	lyric::runtime::Info decodeInfo(const std::vector<uint8_t>& data);

	/**
	 * Encodes an Info into its JSON representation.
	 */
	std::string infoToJson(const lyric::runtime::Info& info);

	/**
	 * Decodes an Info from its JSON representation.
	 * Throws std::runtime_error when the input is not valid Info JSON.
	 */
	lyric::runtime::Info infoFromJson(const std::string& json);

	/**
	 * Sorts lines and their background lines by start time ascending.
	 */
	void sortLinesByTime(lyric::runtime::Info& info);
} // namespace music_lyric_model

#endif
