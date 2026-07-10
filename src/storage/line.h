#ifndef MUSIC_LYRIC_STORAGE_LINE_H_
#define MUSIC_LYRIC_STORAGE_LINE_H_

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "common/time.pb.h"
#include "storage/line/annotation.pb.h"
#include "storage/line/content.pb.h"
#include "storage/word/content.pb.h"

namespace music_lyric_model::storage {
	/**
	 * Creates a LineContent, the sung content shared by a line and its backgrounds.
	 */
	lyric::storage::LineContent makeLineContent(const lyric::storage::LineContent& content = {});

	/**
	 * Creates a Line.
	 */
	lyric::storage::Line makeLine(const lyric::storage::Line& line = {});

	/**
	 * Creates a LineAnnotationUnknown.
	 */
	lyric::storage::LineAnnotationUnknown makeLineAnnotationUnknown(const lyric::storage::LineAnnotationUnknown& unknown = {});

	/**
	 * Creates a LineAnnotationRoman.
	 */
	lyric::storage::LineAnnotationRoman makeLineAnnotationRoman(const lyric::storage::LineAnnotationRoman& roman = {});

	/**
	 * Creates a LineAnnotationTranslate.
	 */
	lyric::storage::LineAnnotationTranslate makeLineAnnotationTranslate(const lyric::storage::LineAnnotationTranslate& translate = {});

	/**
	 * Creates a LineAnnotation, the per-line annotation container.
	 */
	lyric::storage::LineAnnotation makeLineAnnotation(const lyric::storage::LineAnnotation& annotation = {});

	/**
	 * Content of a line, null when absent.
	 */
	const lyric::storage::LineContent* getLineContent(const lyric::storage::Line& line);

	/**
	 * Time range of a line, null when absent.
	 */
	const lyric::common::Time* getLineTime(const lyric::storage::Line& line);

	/**
	 * Duration of a line in milliseconds.
	 */
	int64_t getLineDuration(const lyric::storage::Line& line);

	/**
	 * Words of a line, empty when content is absent.
	 */
	const google::protobuf::RepeatedPtrField<lyric::storage::Word>& getLineWords(const lyric::storage::Line& line);

	/**
	 * Text joined from a list of words.
	 */
	std::string getWordsText(const google::protobuf::RepeatedPtrField<lyric::storage::Word>& words);

	/**
	 * Plain text of a line.
	 */
	std::string getLineText(const lyric::storage::Line& line);

	/**
	 * Distinct language tags among a list of words.
	 */
	std::vector<std::string> getWordsLanguages(const google::protobuf::RepeatedPtrField<lyric::storage::Word>& words);

	/**
	 * Languages of a line, collected from its words.
	 */
	std::vector<std::string> getLineLanguages(const lyric::storage::Line& line);

	/**
	 * Annotation of a line, null when absent.
	 */
	const lyric::storage::LineAnnotation* getLineAnnotation(const lyric::storage::Line& line);

	/**
	 * Index of the line active at the given moment, or -1 when none.
	 */
	int getActiveLineIndex(const google::protobuf::RepeatedPtrField<lyric::storage::Line>& lines, int64_t ms);

	/**
	 * Line active at the given moment, null when none.
	 */
	const lyric::storage::Line* getActiveLine(const google::protobuf::RepeatedPtrField<lyric::storage::Line>& lines, int64_t ms);

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
	std::optional<std::string> getLineTranslate(const lyric::storage::Line& line, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Romanized text of a line, preferring a language match.
	 */
	std::optional<std::string> getLineRoman(const lyric::storage::Line& line, const std::optional<std::string>& language = std::nullopt);
} // namespace music_lyric_model::storage

#endif
