#include "common/unknown.h"

namespace music_lyric_model::common {
	Unknown makeUnknown(Unknown init) {
		return init;
	}

	std::vector<std::string> getUnknownValues(const std::vector<Unknown>& unknowns, const std::string& key) {
		std::vector<std::string> result;
		for (const auto& item : unknowns) {
			if (item.key == key) {
				result.push_back(item.value);
			}
		}
		return result;
	}
} // namespace music_lyric_model::common
