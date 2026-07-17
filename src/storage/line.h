#ifndef MUSIC_LYRIC_MODEL_STORAGE_LINE_H_
#define MUSIC_LYRIC_MODEL_STORAGE_LINE_H_

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "common/line.pb.h"
#include "common/time.pb.h"
#include "common/word.pb.h"
#include "storage/line.pb.h"

namespace music_lyric_model::storage {
	/**
	 * Creates a storage line.
	 */
	lyric::storage::Line makeStorageLine(const lyric::storage::Line& line = {});

	/**
	 * Creates a storage background line.
	 */
	lyric::storage::LineBackground makeStorageLineBackground(const lyric::storage::LineBackground& background = {});

	/**
	 * Time range of a primary line, null when absent.
	 */
	const lyric::common::Time* getStorageLineTime(const lyric::storage::Line& line);

	/**
	 * Time range of a background line, null when absent.
	 */
	const lyric::common::Time* getStorageLineTime(const lyric::storage::LineBackground& line);

	/**
	 * Duration of a primary line in milliseconds.
	 */
	int64_t getStorageLineDuration(const lyric::storage::Line& line);

	/**
	 * Duration of a background line in milliseconds.
	 */
	int64_t getStorageLineDuration(const lyric::storage::LineBackground& line);

	/**
	 * Words of a primary line.
	 */
	const google::protobuf::RepeatedPtrField<lyric::common::Word>& getStorageLineWords(const lyric::storage::Line& line);

	/**
	 * Words of a background line.
	 */
	const google::protobuf::RepeatedPtrField<lyric::common::Word>& getStorageLineWords(const lyric::storage::LineBackground& line);

	/**
	 * Plain text of a primary line.
	 */
	std::string getStorageLineText(const lyric::storage::Line& line);

	/**
	 * Plain text of a background line.
	 */
	std::string getStorageLineText(const lyric::storage::LineBackground& line);

	/**
	 * Languages of a primary line, collected from its words.
	 */
	std::vector<std::string> getStorageLineLanguages(const lyric::storage::Line& line);

	/**
	 * Languages of a background line, collected from its words.
	 */
	std::vector<std::string> getStorageLineLanguages(const lyric::storage::LineBackground& line);

	/**
	 * Annotation of a primary line, null when absent.
	 */
	const lyric::common::LineAnnotation* getStorageLineAnnotation(const lyric::storage::Line& line);

	/**
	 * Annotation of a background line, null when absent.
	 */
	const lyric::common::LineAnnotation* getStorageLineAnnotation(const lyric::storage::LineBackground& line);

	/**
	 * Index of the line active at the given moment, or -1 when none.
	 */
	int getStorageActiveLineIndex(const google::protobuf::RepeatedPtrField<lyric::storage::Line>& lines, int64_t ms);

	/**
	 * Line active at the given moment, null when none.
	 */
	const lyric::storage::Line* getStorageActiveLine(const google::protobuf::RepeatedPtrField<lyric::storage::Line>& lines, int64_t ms);

	/**
	 * Translated text of a primary line, preferring a language match.
	 */
	std::optional<std::string> getStorageLineTranslation(const lyric::storage::Line& line, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Translated text of a background line, preferring a language match.
	 */
	std::optional<std::string> getStorageLineTranslation(const lyric::storage::LineBackground& line, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Romanized text of a primary line, preferring a language match.
	 */
	std::optional<std::string> getStorageLineRoman(const lyric::storage::Line& line, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Romanized text of a background line, preferring a language match.
	 */
	std::optional<std::string> getStorageLineRoman(const lyric::storage::LineBackground& line, const std::optional<std::string>& language = std::nullopt);
} // namespace music_lyric_model::storage

#endif
