#ifndef MUSIC_LYRIC_MODEL_PARSED_INFO_H_
#define MUSIC_LYRIC_MODEL_PARSED_INFO_H_

#include <cstdint>
#include <string>
#include <vector>

#include "model/parsed/info.gen.h"

namespace music_lyric_model::parsed {
	/**
	 * Creates an Info, stamping the current SCHEMA_VERSION.
	 */
	Info makeParsedInfo(Info init = {});

	/**
	 * Whether an Info is marked valid.
	 */
	bool isParsedInfoValid(const Info& info);

	/**
	 * Whether an Info is marked invalid.
	 */
	bool isParsedInfoInvalid(const Info& info);

	/**
	 * Encode an Info to protobuf binary.
	 * Throws std::runtime_error when encoding fails.
	 */
	std::vector<uint8_t> encodeParsedInfo(const Info& info);

	/**
	 * Decode an Info from protobuf binary.
	 * Throws std::runtime_error when the input is not a valid Info.
	 */
	Info decodeParsedInfo(const std::vector<uint8_t>& data);

	/**
	 * Convert an Info to protobuf JSON.
	 * Throws std::runtime_error when encoding fails.
	 */
	std::string parsedInfoToJson(const Info& info);

	/**
	 * Restore an Info from protobuf JSON.
	 * Throws std::runtime_error when the input is not valid Info JSON.
	 */
	Info parsedInfoFromJson(const std::string& json);

	/**
	 * Sort lines and their background lines by start time ascending.
	 */
	void sortParsedLinesByTime(Info& info);
} // namespace music_lyric_model::parsed

#endif
