#ifndef MUSIC_LYRIC_STORAGE_WORD_H_
#define MUSIC_LYRIC_STORAGE_WORD_H_

#include <cstdint>
#include <string>

#include "common/time.pb.h"
#include "storage/word/annotation.pb.h"
#include "storage/word/content.pb.h"

namespace music_lyric_model::storage {
	/**
	 * Creates a normal word wrapped in a Word.
	 */
	lyric::storage::Word makeWordNormal(const lyric::storage::WordNormal& normal = {});

	/**
	 * Creates a run of spaces wrapped in a Word.
	 */
	lyric::storage::Word makeWordSpace(const lyric::storage::WordSpace& space = {});

	/**
	 * Creates a WordAnnotationContent, one token of a word annotation.
	 */
	lyric::storage::WordAnnotationContent makeWordAnnotationContent(const lyric::storage::WordAnnotationContent& content = {});

	/**
	 * Creates a WordAnnotationRoman.
	 */
	lyric::storage::WordAnnotationRoman makeWordAnnotationRoman(const lyric::storage::WordAnnotationRoman& roman = {});

	/**
	 * Creates a WordAnnotationTranslate.
	 */
	lyric::storage::WordAnnotationTranslate makeWordAnnotationTranslate(const lyric::storage::WordAnnotationTranslate& translate = {});

	/**
	 * Creates a WordAnnotationRuby.
	 */
	lyric::storage::WordAnnotationRuby makeWordAnnotationRuby(const lyric::storage::WordAnnotationRuby& ruby = {});

	/**
	 * Creates a WordAnnotationUnknown.
	 */
	lyric::storage::WordAnnotationUnknown makeWordAnnotationUnknown(const lyric::storage::WordAnnotationUnknown& unknown = {});

	/**
	 * Creates a WordAnnotation, the per-word annotation container.
	 */
	lyric::storage::WordAnnotation makeWordAnnotation(const lyric::storage::WordAnnotation& annotation = {});

	/**
	 * Rendered text of a word: its content, or padded spaces.
	 */
	std::string getWordText(const lyric::storage::Word& word);

	/**
	 * Time range of a word, null when absent.
	 */
	const lyric::common::Time* getWordTime(const lyric::storage::Word& word);

	/**
	 * Time range of a bare normal word, null when absent.
	 */
	const lyric::common::Time* getWordTime(const lyric::storage::WordNormal& word);

	/**
	 * Ruby annotation of a word, null when absent.
	 */
	const lyric::storage::WordAnnotationRuby* getWordRuby(const lyric::storage::Word& word);

	/**
	 * Duration of a word in milliseconds.
	 */
	int64_t getWordDuration(const lyric::storage::Word& word);

	/**
	 * Duration of a bare normal word in milliseconds.
	 */
	int64_t getWordDuration(const lyric::storage::WordNormal& word);

	/**
	 * Index of the word active at the given moment, or -1 when none.
	 */
	int getActiveWordIndex(const google::protobuf::RepeatedPtrField<lyric::storage::Word>& words, int64_t ms);

	/**
	 * Word active at the given moment, null when none.
	 */
	const lyric::storage::Word* getActiveWord(const google::protobuf::RepeatedPtrField<lyric::storage::Word>& words, int64_t ms);

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

	/**
	 * Whether a Word holds a normal word.
	 */
	bool isWordNormal(const lyric::storage::Word& word);

	/**
	 * Whether a Word holds a run of spaces.
	 */
	bool isWordSpace(const lyric::storage::Word& word);

	/**
	 * Normal word held by a Word, null when it holds something else.
	 */
	const lyric::storage::WordNormal* asWordNormal(const lyric::storage::Word& word);

	/**
	 * Space run held by a Word, null when it holds something else.
	 */
	const lyric::storage::WordSpace* asWordSpace(const lyric::storage::Word& word);
} // namespace music_lyric_model::storage

#endif
