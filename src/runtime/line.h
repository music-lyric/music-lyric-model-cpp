#ifndef MUSIC_LYRIC_RUNTIME_LINE_H_
#define MUSIC_LYRIC_RUNTIME_LINE_H_

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "common/time.pb.h"
#include "runtime/line/annotation.pb.h"
#include "runtime/line/content.pb.h"
#include "runtime/word/content.pb.h"

namespace music_lyric_model::runtime {
	using namespace lyric::common;
	using namespace lyric::runtime;

	/**
	 * Creates a LineContent, the sung content shared by normal and background lines.
	 */
	lyric::runtime::LineContent makeLineContent(const lyric::runtime::LineContent& content = {});

	/**
	 * Creates a normal line wrapped in a Line, with an optional time range.
	 */
	lyric::runtime::Line makeLineNormal(const lyric::runtime::LineNormal& normal = {}, const lyric::common::Time* time = nullptr);

	/**
	 * Creates a LineBackground, a background line attached to a normal line.
	 */
	lyric::runtime::LineBackground makeLineBackground(const lyric::runtime::LineBackground& background = {});

	/**
	 * Creates an interlude wrapped in a Line, with an optional time range.
	 */
	lyric::runtime::Line makeLineInterlude(const lyric::common::Time* time = nullptr);

	/**
	 * Creates a LineAnnotationUnknown.
	 */
	lyric::runtime::LineAnnotationUnknown makeLineAnnotationUnknown(const lyric::runtime::LineAnnotationUnknown& unknown = {});

	/**
	 * Creates a LineAnnotationRoman.
	 */
	lyric::runtime::LineAnnotationRoman makeLineAnnotationRoman(const lyric::runtime::LineAnnotationRoman& roman = {});

	/**
	 * Creates a LineAnnotationTranslate.
	 */
	lyric::runtime::LineAnnotationTranslate makeLineAnnotationTranslate(const lyric::runtime::LineAnnotationTranslate& translate = {});

	/**
	 * Creates a LineAnnotation, the per-line annotation container.
	 */
	lyric::runtime::LineAnnotation makeLineAnnotation(const lyric::runtime::LineAnnotation& annotation = {});

	/**
	 * Whether a Line holds a normal line.
	 */
	bool isLineNormal(const lyric::runtime::Line& line);

	/**
	 * Whether a Line holds an interlude.
	 */
	bool isLineInterlude(const lyric::runtime::Line& line);

	/**
	 * Normal line held by a Line, null when it holds something else.
	 */
	const lyric::runtime::LineNormal* asLineNormal(const lyric::runtime::Line& line);

	/**
	 * Interlude held by a Line, null when it holds something else.
	 */
	const lyric::runtime::LineInterlude* asLineInterlude(const lyric::runtime::Line& line);

	/**
	 * Time range of a line, null when absent.
	 */
	const lyric::common::Time* getLineTime(const lyric::runtime::Line& line);

	/**
	 * Duration of a line in milliseconds.
	 */
	int64_t getLineDuration(const lyric::runtime::Line& line);

	/**
	 * Content of a line, null on an interlude.
	 */
	const lyric::runtime::LineContent* getLineContent(const lyric::runtime::Line& line);

	/**
	 * Words of a line, empty for an interlude.
	 */
	const google::protobuf::RepeatedPtrField<lyric::runtime::Word>& getLineWords(const lyric::runtime::Line& line);

	/**
	 * Text joined from a list of words.
	 */
	std::string getWordsText(const google::protobuf::RepeatedPtrField<lyric::runtime::Word>& words);

	/**
	 * Plain text of a line.
	 */
	std::string getLineText(const lyric::runtime::Line& line);

	/**
	 * Distinct language tags among a list of words.
	 */
	std::vector<std::string> getWordsLanguages(const google::protobuf::RepeatedPtrField<lyric::runtime::Word>& words);

	/**
	 * Languages of a line: the explicit tags, otherwise those of its words.
	 */
	std::vector<std::string> getLineLanguages(const lyric::runtime::Line& line);

	/**
	 * Annotation of a line, null on an interlude.
	 */
	const lyric::runtime::LineAnnotation* getLineAnnotation(const lyric::runtime::Line& line);

	/**
	 * Index of the line active at the given moment, or -1 when none.
	 */
	int getActiveLineIndex(const google::protobuf::RepeatedPtrField<lyric::runtime::Line>& lines, int64_t ms);

	/**
	 * Line active at the given moment, null when none.
	 */
	const lyric::runtime::Line* getActiveLine(const google::protobuf::RepeatedPtrField<lyric::runtime::Line>& lines, int64_t ms);

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
	std::optional<std::string> getLineTranslate(const lyric::runtime::Line& line, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Romanized text of a line, preferring a language match.
	 */
	std::optional<std::string> getLineRoman(const lyric::runtime::Line& line, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Line romanizations aggregated from a line's word annotations, grouped by language.
	 */
	std::vector<lyric::runtime::LineAnnotationRoman> deriveLineRomans(const google::protobuf::RepeatedPtrField<lyric::runtime::Word>& words);

	/**
	 * Line translations aggregated from a line's word annotations, grouped by language.
	 */
	std::vector<lyric::runtime::LineAnnotationTranslate> deriveLineTranslates(const google::protobuf::RepeatedPtrField<lyric::runtime::Word>& words);

	/**
	 * Line unknown annotations aggregated from a line's word annotations, grouped by original key.
	 */
	std::vector<lyric::runtime::LineAnnotationUnknown> deriveLineUnknowns(const google::protobuf::RepeatedPtrField<lyric::runtime::Word>& words);

	/**
	 * Builds a LineAnnotation by aggregating a line's word annotations; every item is marked derived.
	 */
	lyric::runtime::LineAnnotation deriveLineAnnotation(const google::protobuf::RepeatedPtrField<lyric::runtime::Word>& words);
} // namespace music_lyric_model::runtime

#endif
