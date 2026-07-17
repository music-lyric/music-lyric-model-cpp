#ifndef MUSIC_LYRIC_MODEL_STORAGE_LINE_H_
#define MUSIC_LYRIC_MODEL_STORAGE_LINE_H_

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/line.h"
#include "common/part.h"
#include "common/time.h"
#include "common/word.h"

namespace music_lyric_model::storage {
	/**
	 * Background vocal line attached to a primary line.
	 */
	struct LineBackground {
		std::unordered_map<std::string, std::string> extra;
		std::optional<common::Time>                  time;
		std::vector<std::string>                     agents;
		std::vector<common::Word>                    words;
		std::optional<common::LineAnnotation>        annotation;
	};

	/**
	 * Primary storage lyric line.
	 */
	struct Line {
		std::unordered_map<std::string, std::string> extra;
		std::optional<common::Time>                  time;
		std::optional<common::Part>                  part;
		std::vector<std::string>                     agents;
		std::vector<common::Word>                    words;
		std::optional<common::LineAnnotation>        annotation;
		std::vector<LineBackground>                  backgrounds;
	};

	/**
	 * Time range of a primary line, null when absent.
	 */
	const common::Time* getStorageLineTime(const Line& line);

	/**
	 * Time range of a background line, null when absent.
	 */
	const common::Time* getStorageLineTime(const LineBackground& line);

	/**
	 * Duration of a primary line in milliseconds.
	 */
	int64_t getStorageLineDuration(const Line& line);

	/**
	 * Duration of a background line in milliseconds.
	 */
	int64_t getStorageLineDuration(const LineBackground& line);

	/**
	 * Words of a primary line.
	 */
	const std::vector<common::Word>& getStorageLineWords(const Line& line);

	/**
	 * Words of a background line.
	 */
	const std::vector<common::Word>& getStorageLineWords(const LineBackground& line);

	/**
	 * Plain text of a primary line.
	 */
	std::string getStorageLineText(const Line& line);

	/**
	 * Plain text of a background line.
	 */
	std::string getStorageLineText(const LineBackground& line);

	/**
	 * Languages of a primary line, collected from its words.
	 */
	std::vector<std::string> getStorageLineLanguages(const Line& line);

	/**
	 * Languages of a background line, collected from its words.
	 */
	std::vector<std::string> getStorageLineLanguages(const LineBackground& line);

	/**
	 * Annotation of a primary line, null when absent.
	 */
	const common::LineAnnotation* getStorageLineAnnotation(const Line& line);

	/**
	 * Annotation of a background line, null when absent.
	 */
	const common::LineAnnotation* getStorageLineAnnotation(const LineBackground& line);

	/**
	 * Index of the line active at the given moment, or -1 when none.
	 */
	int getStorageActiveLineIndex(const std::vector<Line>& lines, int64_t ms);

	/**
	 * Line active at the given moment, null when none.
	 */
	const Line* getStorageActiveLine(const std::vector<Line>& lines, int64_t ms);

	/**
	 * Translated text of a primary line, preferring a language match.
	 */
	std::optional<std::string> getStorageLineTranslation(const Line& line, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Translated text of a background line, preferring a language match.
	 */
	std::optional<std::string> getStorageLineTranslation(const LineBackground& line, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Romanized text of a primary line, preferring a language match.
	 */
	std::optional<std::string> getStorageLineRoman(const Line& line, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Romanized text of a background line, preferring a language match.
	 */
	std::optional<std::string> getStorageLineRoman(const LineBackground& line, const std::optional<std::string>& language = std::nullopt);
} // namespace music_lyric_model::storage

#endif
