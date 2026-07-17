#include "common/unknown.h"

namespace music_lyric_model::common {
	lyric::common::Unknown makeUnknown(const lyric::common::Unknown& unknown) {
		return unknown;
	}

	std::vector<std::string> getUnknownValues(const google::protobuf::RepeatedPtrField<lyric::common::Unknown>& unknowns, const std::string& key) {
		std::vector<std::string> result;
		for (const auto& item : unknowns) {
			if (item.key() == key) {
				result.push_back(item.value());
			}
		}
		return result;
	}
} // namespace music_lyric_model::common
