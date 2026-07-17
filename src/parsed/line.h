#ifndef MUSIC_LYRIC_MODEL_PARSED_LINE_H_
#define MUSIC_LYRIC_MODEL_PARSED_LINE_H_

#include "model/parsed/line.gen.h"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "common/line.h"
#include "common/time.h"
#include "common/unknown.h"
#include "common/word.h"

namespace music_lyric_model::parsed {
	/**
	 * Creates a normal line wrapped in a Line.
	 */
	Line makeParsedLineNormal(LineNormal normal = {});

	/**
	 * Creates an interlude wrapped in a Line.
	 */
	Line makeParsedLineInterlude(LineInterlude interlude = {});

	/**
	 * Whether a Line holds a normal line.
	 */
	bool isParsedLineNormal(const Line& line);

	/**
	 * Whether a Line holds an interlude.
	 */
	bool isParsedLineInterlude(const Line& line);

	/**
	 * Normal body of a line, null when it holds something else.
	 */
	const LineNormal* asParsedLineNormal(const Line& line);

	/**
	 * Interlude body of a line, null when it holds something else.
	 */
	const LineInterlude* asParsedLineInterlude(const Line& line);

	/**
	 * Time range of a line wrapper, null when absent.
	 */
	const common::Time* getParsedLineTime(const Line& line);

	/**
	 * Time range of a normal body, null when absent.
	 */
	const common::Time* getParsedLineTime(const LineNormal& line);

	/**
	 * Time range of a background line, null when absent.
	 */
	const common::Time* getParsedLineTime(const LineBackground& line);

	/**
	 * Time range of an interlude body, null when absent.
	 */
	const common::Time* getParsedLineTime(const LineInterlude& line);

	/**
	 * Duration of a line wrapper in milliseconds.
	 */
	int64_t getParsedLineDuration(const Line& line);

	/**
	 * Duration of a normal body in milliseconds.
	 */
	int64_t getParsedLineDuration(const LineNormal& line);

	/**
	 * Duration of a background line in milliseconds.
	 */
	int64_t getParsedLineDuration(const LineBackground& line);

	/**
	 * Duration of an interlude body in milliseconds.
	 */
	int64_t getParsedLineDuration(const LineInterlude& line);

	/**
	 * Words of a line wrapper; empty for an interlude.
	 */
	const std::vector<common::Word>& getParsedLineWords(const Line& line);

	/**
	 * Words of a normal body.
	 */
	const std::vector<common::Word>& getParsedLineWords(const LineNormal& line);

	/**
	 * Words of a background line.
	 */
	const std::vector<common::Word>& getParsedLineWords(const LineBackground& line);

	/**
	 * Plain text of a line wrapper.
	 */
	std::string getParsedLineText(const Line& line);

	/**
	 * Plain text of a normal body.
	 */
	std::string getParsedLineText(const LineNormal& line);

	/**
	 * Plain text of a background line.
	 */
	std::string getParsedLineText(const LineBackground& line);

	/**
	 * Languages of a line: explicit tags when present, otherwise those of its words.
	 * Empty for an interlude wrapper.
	 */
	std::vector<std::string> getParsedLineLanguages(const Line& line);

	/**
	 * Languages of a normal body: explicit tags when present, otherwise those of its words.
	 */
	std::vector<std::string> getParsedLineLanguages(const LineNormal& line);

	/**
	 * Languages of a background line: explicit tags when present, otherwise those of its words.
	 */
	std::vector<std::string> getParsedLineLanguages(const LineBackground& line);

	/**
	 * Annotation of a line wrapper; null on an interlude.
	 */
	const common::LineAnnotation* getParsedLineAnnotation(const Line& line);

	/**
	 * Annotation of a normal body; null when absent.
	 */
	const common::LineAnnotation* getParsedLineAnnotation(const LineNormal& line);

	/**
	 * Annotation of a background line; null when absent.
	 */
	const common::LineAnnotation* getParsedLineAnnotation(const LineBackground& line);

	/**
	 * Index of the line active at the given moment, or -1 when none.
	 */
	int getParsedActiveLineIndex(const std::vector<Line>& lines, int64_t ms);

	/**
	 * Line active at the given moment, null when none.
	 */
	const Line* getParsedActiveLine(const std::vector<Line>& lines, int64_t ms);

	/**
	 * Translated text of a line wrapper, preferring a language match.
	 */
	std::optional<std::string> getParsedLineTranslation(const Line& line, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Translated text of a normal body, preferring a language match.
	 */
	std::optional<std::string> getParsedLineTranslation(const LineNormal& line, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Translated text of a background line, preferring a language match.
	 */
	std::optional<std::string> getParsedLineTranslation(const LineBackground& line, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Romanized text of a line wrapper, preferring a language match.
	 */
	std::optional<std::string> getParsedLineRoman(const Line& line, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Romanized text of a normal body, preferring a language match.
	 */
	std::optional<std::string> getParsedLineRoman(const LineNormal& line, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Romanized text of a background line, preferring a language match.
	 */
	std::optional<std::string> getParsedLineRoman(const LineBackground& line, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Line romanizations aggregated from word annotations, grouped by language.
	 * Display-time only; does not write back into the line.
	 */
	std::vector<common::LineAnnotationRoman> deriveParsedLineRomans(const std::vector<common::Word>& words);

	/**
	 * Line translations aggregated from word annotations, grouped by language.
	 * Display-time only; does not write back into the line.
	 */
	std::vector<common::LineAnnotationTranslation> deriveParsedLineTranslations(const std::vector<common::Word>& words);

	/**
	 * Line unknown annotations aggregated from word annotations, grouped by original key.
	 * Display-time only; does not write back into the line.
	 */
	std::vector<common::Unknown> deriveParsedLineUnknowns(const std::vector<common::Word>& words);

	/**
	 * Build a LineAnnotation by aggregating word annotations for display.
	 * Does not write back into the line.
	 */
	common::LineAnnotation deriveParsedLineAnnotation(const std::vector<common::Word>& words);
} // namespace music_lyric_model::parsed

#endif
