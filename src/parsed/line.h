#ifndef MUSIC_LYRIC_MODEL_PARSED_LINE_H_
#define MUSIC_LYRIC_MODEL_PARSED_LINE_H_

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "common/line.pb.h"
#include "common/time.pb.h"
#include "common/unknown.pb.h"
#include "common/word.pb.h"
#include "parsed/line.pb.h"

namespace music_lyric_model::parsed {
	/**
	 * Creates a normal line wrapped in a Line.
	 */
	lyric::parsed::Line makeParsedLineNormal(const lyric::parsed::LineNormal& normal = {});

	/**
	 * Creates a LineBackground.
	 */
	lyric::parsed::LineBackground makeParsedLineBackground(const lyric::parsed::LineBackground& background = {});

	/**
	 * Creates an interlude wrapped in a Line.
	 */
	lyric::parsed::Line makeParsedLineInterlude(const lyric::parsed::LineInterlude& interlude = {});

	/**
	 * Whether a Line holds a normal line.
	 */
	bool isParsedLineNormal(const lyric::parsed::Line& line);

	/**
	 * Whether a Line holds an interlude.
	 */
	bool isParsedLineInterlude(const lyric::parsed::Line& line);

	/**
	 * Normal body of a line, null when it holds something else.
	 */
	const lyric::parsed::LineNormal* asParsedLineNormal(const lyric::parsed::Line& line);

	/**
	 * Interlude body of a line, null when it holds something else.
	 */
	const lyric::parsed::LineInterlude* asParsedLineInterlude(const lyric::parsed::Line& line);

	/**
	 * Time range of a line wrapper, null when absent.
	 */
	const lyric::common::Time* getParsedLineTime(const lyric::parsed::Line& line);

	/**
	 * Time range of a normal body, null when absent.
	 */
	const lyric::common::Time* getParsedLineTime(const lyric::parsed::LineNormal& line);

	/**
	 * Time range of a background line, null when absent.
	 */
	const lyric::common::Time* getParsedLineTime(const lyric::parsed::LineBackground& line);

	/**
	 * Time range of an interlude body, null when absent.
	 */
	const lyric::common::Time* getParsedLineTime(const lyric::parsed::LineInterlude& line);

	/**
	 * Duration of a line wrapper in milliseconds.
	 */
	int64_t getParsedLineDuration(const lyric::parsed::Line& line);

	/**
	 * Duration of a normal body in milliseconds.
	 */
	int64_t getParsedLineDuration(const lyric::parsed::LineNormal& line);

	/**
	 * Duration of a background line in milliseconds.
	 */
	int64_t getParsedLineDuration(const lyric::parsed::LineBackground& line);

	/**
	 * Duration of an interlude body in milliseconds.
	 */
	int64_t getParsedLineDuration(const lyric::parsed::LineInterlude& line);

	/**
	 * Words of a line wrapper; empty for an interlude.
	 */
	const google::protobuf::RepeatedPtrField<lyric::common::Word>& getParsedLineWords(const lyric::parsed::Line& line);

	/**
	 * Words of a normal body.
	 */
	const google::protobuf::RepeatedPtrField<lyric::common::Word>& getParsedLineWords(const lyric::parsed::LineNormal& line);

	/**
	 * Words of a background line.
	 */
	const google::protobuf::RepeatedPtrField<lyric::common::Word>& getParsedLineWords(const lyric::parsed::LineBackground& line);

	/**
	 * Plain text of a line wrapper.
	 */
	std::string getParsedLineText(const lyric::parsed::Line& line);

	/**
	 * Plain text of a normal body.
	 */
	std::string getParsedLineText(const lyric::parsed::LineNormal& line);

	/**
	 * Plain text of a background line.
	 */
	std::string getParsedLineText(const lyric::parsed::LineBackground& line);

	/**
	 * Languages of a line: explicit tags when present, otherwise those of its words.
	 * Empty for an interlude wrapper.
	 */
	std::vector<std::string> getParsedLineLanguages(const lyric::parsed::Line& line);

	/**
	 * Languages of a normal body: explicit tags when present, otherwise those of its words.
	 */
	std::vector<std::string> getParsedLineLanguages(const lyric::parsed::LineNormal& line);

	/**
	 * Languages of a background line: explicit tags when present, otherwise those of its words.
	 */
	std::vector<std::string> getParsedLineLanguages(const lyric::parsed::LineBackground& line);

	/**
	 * Annotation of a line wrapper; null on an interlude.
	 */
	const lyric::common::LineAnnotation* getParsedLineAnnotation(const lyric::parsed::Line& line);

	/**
	 * Annotation of a normal body; null when absent.
	 */
	const lyric::common::LineAnnotation* getParsedLineAnnotation(const lyric::parsed::LineNormal& line);

	/**
	 * Annotation of a background line; null when absent.
	 */
	const lyric::common::LineAnnotation* getParsedLineAnnotation(const lyric::parsed::LineBackground& line);

	/**
	 * Index of the line active at the given moment, or -1 when none.
	 */
	int getParsedActiveLineIndex(const google::protobuf::RepeatedPtrField<lyric::parsed::Line>& lines, int64_t ms);

	/**
	 * Line active at the given moment, null when none.
	 */
	const lyric::parsed::Line* getParsedActiveLine(const google::protobuf::RepeatedPtrField<lyric::parsed::Line>& lines, int64_t ms);

	/**
	 * Translated text of a line wrapper, preferring a language match.
	 */
	std::optional<std::string> getParsedLineTranslation(const lyric::parsed::Line& line, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Translated text of a normal body, preferring a language match.
	 */
	std::optional<std::string> getParsedLineTranslation(const lyric::parsed::LineNormal& line, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Translated text of a background line, preferring a language match.
	 */
	std::optional<std::string> getParsedLineTranslation(const lyric::parsed::LineBackground& line, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Romanized text of a line wrapper, preferring a language match.
	 */
	std::optional<std::string> getParsedLineRoman(const lyric::parsed::Line& line, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Romanized text of a normal body, preferring a language match.
	 */
	std::optional<std::string> getParsedLineRoman(const lyric::parsed::LineNormal& line, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Romanized text of a background line, preferring a language match.
	 */
	std::optional<std::string> getParsedLineRoman(const lyric::parsed::LineBackground& line, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Line romanizations aggregated from word annotations, grouped by language.
	 * Display-time only; does not write back into the line message.
	 */
	std::vector<lyric::common::LineAnnotationRoman> deriveParsedLineRomans(const google::protobuf::RepeatedPtrField<lyric::common::Word>& words);

	/**
	 * Line translations aggregated from word annotations, grouped by language.
	 * Display-time only; does not write back into the line message.
	 */
	std::vector<lyric::common::LineAnnotationTranslation> deriveParsedLineTranslations(const google::protobuf::RepeatedPtrField<lyric::common::Word>& words);

	/**
	 * Line unknown annotations aggregated from word annotations, grouped by original key.
	 * Display-time only; does not write back into the line message.
	 */
	std::vector<lyric::common::Unknown> deriveParsedLineUnknowns(const google::protobuf::RepeatedPtrField<lyric::common::Word>& words);

	/**
	 * Build a LineAnnotation by aggregating word annotations for display.
	 * Does not write back into the line message.
	 */
	lyric::common::LineAnnotation deriveParsedLineAnnotation(const google::protobuf::RepeatedPtrField<lyric::common::Word>& words);
} // namespace music_lyric_model::parsed

#endif
