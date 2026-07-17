#ifndef MUSIC_LYRIC_MODEL_COMMON_LINE_H_
#define MUSIC_LYRIC_MODEL_COMMON_LINE_H_

#include "model/common/line.gen.h"

#include <optional>
#include <string>
#include <vector>

namespace music_lyric_model::common {
	/**
	 * First annotation item, preferring a language match.
	 */
	template <typename T>
	const T* getFirstAnnotation(const std::vector<T>& items, const std::optional<std::string>& language = std::nullopt) {
		if (language.has_value()) {
			for (const auto& item : items) {
				if (item.language.has_value() && *item.language == *language) {
					return &item;
				}
			}
		}
		return items.empty() ? nullptr : &items[0];
	}
} // namespace music_lyric_model::common

#endif
