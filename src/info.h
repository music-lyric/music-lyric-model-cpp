#ifndef MUSIC_LYRIC_INFO_H_
#define MUSIC_LYRIC_INFO_H_

#include <cstdint>
#include <string>
#include <vector>

#include "info.pb.h"

namespace music_lyric_model {
	using namespace lyric;

	/**
	 * Creates an Info stamped with the current SCHEMA_VERSION.
	 */
	lyric::Info makeInfo();

	/**
	 * Encodes an Info into protobuf binary wire format.
	 */
	std::vector<uint8_t> encodeInfoWithBinary(const lyric::Info &info);

	/**
	 * Decodes an Info from protobuf binary wire format.
	 * Throws std::runtime_error when the input is not a valid Info.
	 */
	lyric::Info decodeInfoWithBinary(const std::vector<uint8_t> &data);

	/**
	 * Encodes an Info into its JSON representation.
	 */
	std::string encodeInfoWithJson(const lyric::Info &info);

	/**
	 * Decodes an Info from its JSON representation.
	 * Throws std::runtime_error when the input is not valid Info JSON.
	 */
	lyric::Info decodeInfoWithJson(const std::string &json);
}

#endif
