#ifndef MUSIC_LYRIC_LINE_ANNOTATION_H_
#define MUSIC_LYRIC_LINE_ANNOTATION_H_

#include <optional>
#include <string>
#include <vector>

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

	/**
	 * Translated text of a line, preferring a language match.
	 */
	std::optional<std::string> getLineTranslate(const lyric::Line& line, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Romanized text of a line, preferring a language match.
	 */
	std::optional<std::string> getLineRoman(const lyric::Line& line, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Line romanizations aggregated from a line's word annotations, grouped by language.
	 */
	std::vector<lyric::LineAnnotationRoman> deriveLineRomans(const google::protobuf::RepeatedPtrField<lyric::Word>& words);

	/**
	 * Line translations aggregated from a line's word annotations, grouped by language.
	 */
	std::vector<lyric::LineAnnotationTranslate> deriveLineTranslates(const google::protobuf::RepeatedPtrField<lyric::Word>& words);

	/**
	 * Line unknown annotations aggregated from a line's word annotations, grouped by original key.
	 */
	std::vector<lyric::LineAnnotationUnknown> deriveLineUnknowns(const google::protobuf::RepeatedPtrField<lyric::Word>& words);

	/**
	 * Builds a LineAnnotation by aggregating a line's word annotations; every item is marked derived.
	 */
	lyric::LineAnnotation deriveLineAnnotation(const google::protobuf::RepeatedPtrField<lyric::Word>& words);
} // namespace music_lyric_model

#endif
