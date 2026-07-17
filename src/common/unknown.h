#ifndef MUSIC_LYRIC_MODEL_COMMON_UNKNOWN_H_
#define MUSIC_LYRIC_MODEL_COMMON_UNKNOWN_H_

#include <string>
#include <vector>

namespace music_lyric_model::common {
	/**
	 * A key-value pair preserved from an unrecognized source field.
	 */
	struct Unknown {
		std::string key;
		std::string value;
	};

	/**
	 * Raw values among unknowns that carry the given original key.
	 */
	std::vector<std::string> getUnknownValues(const std::vector<Unknown>& unknowns, const std::string& key);
} // namespace music_lyric_model::common

#endif
