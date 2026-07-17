#ifndef MUSIC_LYRIC_MODEL_COMMON_UNKNOWN_H_
#define MUSIC_LYRIC_MODEL_COMMON_UNKNOWN_H_

#include <string>
#include <vector>

#include "common/unknown.pb.h"

namespace music_lyric_model::common {
	/**
	 * Creates an Unknown key-value pair preserved from an unrecognized source field.
	 */
	lyric::common::Unknown makeUnknown(const lyric::common::Unknown& unknown = {});

	/**
	 * Raw values among unknowns that carry the given original key.
	 */
	std::vector<std::string> getUnknownValues(const google::protobuf::RepeatedPtrField<lyric::common::Unknown>& unknowns, const std::string& key);
} // namespace music_lyric_model::common

#endif
