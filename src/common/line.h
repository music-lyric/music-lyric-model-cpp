#ifndef MUSIC_LYRIC_MODEL_COMMON_LINE_H_
#define MUSIC_LYRIC_MODEL_COMMON_LINE_H_

#include <optional>
#include <string>
#include <vector>

#include "unknown.h"

namespace music_lyric_model::common {
	/**
	 * A romanized line annotation item.
	 */
	struct LineAnnotationRoman {
		std::optional<std::string> language;
		std::string                content;
	};

	/**
	 * A translated line annotation item.
	 */
	struct LineAnnotationTranslation {
		std::optional<std::string> language;
		std::string                content;
	};

	/**
	 * Line-level annotations explicitly present in the source.
	 */
	struct LineAnnotation {
		std::vector<Unknown>                    unknowns;
		std::vector<LineAnnotationRoman>        romans;
		std::vector<LineAnnotationTranslation>  translations;
	};

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
