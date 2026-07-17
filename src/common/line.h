#ifndef MUSIC_LYRIC_MODEL_COMMON_LINE_H_
#define MUSIC_LYRIC_MODEL_COMMON_LINE_H_

#include <optional>
#include <string>

#include "common/line.pb.h"

namespace music_lyric_model::common {
	/**
	 * Creates a LineAnnotationRoman.
	 */
	lyric::common::LineAnnotationRoman makeLineAnnotationRoman(const lyric::common::LineAnnotationRoman& roman = {});

	/**
	 * Creates a LineAnnotationTranslation.
	 */
	lyric::common::LineAnnotationTranslation makeLineAnnotationTranslation(const lyric::common::LineAnnotationTranslation& translation = {});

	/**
	 * Creates a LineAnnotation, the per-line annotation container.
	 */
	lyric::common::LineAnnotation makeLineAnnotation(const lyric::common::LineAnnotation& annotation = {});

	/**
	 * First annotation item, preferring a language match.
	 */
	template <typename T>
	const T* getFirstAnnotation(const google::protobuf::RepeatedPtrField<T>& items, const std::optional<std::string>& language = std::nullopt) {
		if (language.has_value()) {
			for (const auto& item : items) {
				if (item.has_language() && item.language() == *language) {
					return &item;
				}
			}
		}
		return items.empty() ? nullptr : &items.Get(0);
	}
} // namespace music_lyric_model::common

#endif
