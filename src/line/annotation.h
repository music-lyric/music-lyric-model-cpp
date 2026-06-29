#ifndef MUSIC_LYRIC_LINE_ANNOTATION_H_
#define MUSIC_LYRIC_LINE_ANNOTATION_H_

#include <optional>
#include <string>

#include "line/annotation.pb.h"
#include "line/content.pb.h"

namespace music_lyric_model {
	using namespace lyric;

	/**
	 * Creates a LineAnnotationUnknown.
	 */
	lyric::LineAnnotationUnknown makeLineAnnotationUnknown();

	/**
	 * Creates a LineAnnotationRoman.
	 */
	lyric::LineAnnotationRoman makeLineAnnotationRoman();

	/**
	 * Creates a LineAnnotationTranslate.
	 */
	lyric::LineAnnotationTranslate makeLineAnnotationTranslate();

	/**
	 * Creates a LineAnnotation, the per-line annotation container.
	 */
	lyric::LineAnnotation makeLineAnnotation();

	/**
	 * First annotation item, preferring a language match.
	 */
	template <typename T>
	const T *getFirstAnnotation(const google::protobuf::RepeatedPtrField<T> &items, const std::optional<std::string> &language = std::nullopt) {
		if (language.has_value()) {
			for (const auto &item : items) {
				if (item.has_language() && item.language() == *language) {
					return &item;
				}
			}
		}
		return items.empty() ? nullptr : &items.Get(0);
	}

	/**
	 * Translated text of a line, preferring a language match.
	 */
	std::optional<std::string> getLineTranslate(const lyric::Line &line, const std::optional<std::string> &language = std::nullopt);

	/**
	 * Romanized text of a line, preferring a language match.
	 */
	std::optional<std::string> getLineRoman(const lyric::Line &line, const std::optional<std::string> &language = std::nullopt);
}

#endif
