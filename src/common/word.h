#ifndef MUSIC_LYRIC_COMMON_WORD_H_
#define MUSIC_LYRIC_COMMON_WORD_H_

#include <cstdint>
#include <string>
#include <vector>

#include "common/time.pb.h"
#include "common/word.pb.h"

namespace music_lyric_model::common {
	/**
	 * Creates a normal word wrapped in a Word.
	 */
	lyric::common::Word makeWordNormal(const lyric::common::WordNormal& normal = {});

	/**
	 * Creates a run of spaces wrapped in a Word.
	 */
	lyric::common::Word makeWordSpace(const lyric::common::WordSpace& space = {});

	/**
	 * Creates a WordAnnotationContent, one token of a word annotation.
	 */
	lyric::common::WordAnnotationContent makeWordAnnotationContent(const lyric::common::WordAnnotationContent& content = {});

	/**
	 * Creates a WordAnnotationRoman.
	 */
	lyric::common::WordAnnotationRoman makeWordAnnotationRoman(const lyric::common::WordAnnotationRoman& roman = {});

	/**
	 * Creates a WordAnnotationTranslation.
	 */
	lyric::common::WordAnnotationTranslation makeWordAnnotationTranslation(const lyric::common::WordAnnotationTranslation& translation = {});

	/**
	 * Creates a WordAnnotationRuby.
	 */
	lyric::common::WordAnnotationRuby makeWordAnnotationRuby(const lyric::common::WordAnnotationRuby& ruby = {});

	/**
	 * Creates a WordAnnotation, the per-word annotation container.
	 */
	lyric::common::WordAnnotation makeWordAnnotation(const lyric::common::WordAnnotation& annotation = {});

	/**
	 * Whether a Word holds a normal word.
	 */
	bool isWordNormal(const lyric::common::Word& word);

	/**
	 * Whether a Word holds a run of spaces.
	 */
	bool isWordSpace(const lyric::common::Word& word);

	/**
	 * Normal word held by a Word, null when it holds something else.
	 */
	const lyric::common::WordNormal* asWordNormal(const lyric::common::Word& word);

	/**
	 * Space run held by a Word, null when it holds something else.
	 */
	const lyric::common::WordSpace* asWordSpace(const lyric::common::Word& word);

	/**
	 * Rendered text of a word: its content, or padded spaces.
	 */
	std::string getWordText(const lyric::common::Word& word);

	/**
	 * Text joined from words in order.
	 */
	std::string getWordsText(const google::protobuf::RepeatedPtrField<lyric::common::Word>& words);

	/**
	 * Distinct language tags among a list of words, in first-seen order.
	 */
	std::vector<std::string> getWordsLanguages(const google::protobuf::RepeatedPtrField<lyric::common::Word>& words);

	/**
	 * Time range of a word, null when absent.
	 */
	const lyric::common::Time* getWordTime(const lyric::common::Word& word);

	/**
	 * Time range of a bare normal word, null when absent.
	 */
	const lyric::common::Time* getWordTime(const lyric::common::WordNormal& word);

	/**
	 * Ruby annotations of a normal word.
	 */
	std::vector<const lyric::common::WordAnnotationRuby*> getWordRubies(const lyric::common::Word& word);

	/**
	 * Duration of a word in milliseconds.
	 */
	int64_t getWordDuration(const lyric::common::Word& word);

	/**
	 * Duration of a bare normal word in milliseconds.
	 */
	int64_t getWordDuration(const lyric::common::WordNormal& word);

	/**
	 * Index of the word active at the given moment, or -1 when none.
	 */
	int getActiveWordIndex(const google::protobuf::RepeatedPtrField<lyric::common::Word>& words, int64_t ms);

	/**
	 * Word active at the given moment, null when none.
	 */
	const lyric::common::Word* getActiveWord(const google::protobuf::RepeatedPtrField<lyric::common::Word>& words, int64_t ms);

	/**
	 * Text joined from every annotation token.
	 */
	template <typename T>
	std::string getWordAnnotationText(const T& item) {
		std::string result;
		for (const auto& word : item.words()) {
			result += word.content();
		}
		return result;
	}
} // namespace music_lyric_model::common

#endif
