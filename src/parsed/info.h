#ifndef MUSIC_LYRIC_MODEL_PARSED_INFO_H_
#define MUSIC_LYRIC_MODEL_PARSED_INFO_H_

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/agent.h"
#include "common/common.h"
#include "common/meta.h"
#include "language.h"
#include "line.h"

namespace music_lyric_model::parsed {
	/**
	 * Parse validity of a lyric.
	 */
	enum class InfoType {
		Unspecified = 0,
		Invalid     = 1,
		Valid       = 2,
	};

	/**
	 * A fully parsed lyric document.
	 */
	struct Info {
		std::string                                  version;
		InfoType                                     type = InfoType::Unspecified;
		common::Timing                               timing = common::Timing::Unspecified;
		std::unordered_map<std::string, std::string> extra;
		common::Meta                                 meta;
		std::vector<LanguageItem>                    languages;
		std::vector<common::AgentItem>               agents;
		std::vector<Line>                            lines;
	};

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
