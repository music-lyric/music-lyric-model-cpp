#ifndef MUSIC_LYRIC_MODEL_COMMON_LINE_H_
#define MUSIC_LYRIC_MODEL_COMMON_LINE_H_

#include <optional>
#include <string>
#include <vector>

#include "model/common/line.gen.h"

namespace music_lyric_model::common {
	/**
	 * Creates a LineAnnotationRoman.
	 */
	LineAnnotationRoman makeLineAnnotationRoman(LineAnnotationRoman init = {});

	/**
	 * Creates a LineAnnotationTranslation.
	 */
	LineAnnotationTranslation makeLineAnnotationTranslation(LineAnnotationTranslation init = {});

	/**
	 * Creates a LineAnnotation, the per-line annotation container.
	 */
	LineAnnotation makeLineAnnotation(LineAnnotation init = {});

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
