#ifndef MUSIC_LYRIC_MODEL_STORAGE_LINE_H_
#define MUSIC_LYRIC_MODEL_STORAGE_LINE_H_

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "common/line.h"
#include "common/time.h"
#include "common/word.h"
#include "model/storage/line.gen.h"

namespace music_lyric_model::storage {
	/**
	 * Creates a storage line.
	 */
	Line makeStorageLine(Line init = {});

	/**
	 * Creates a storage background line.
	 */
	LineBackground makeStorageLineBackground(LineBackground init = {});

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
