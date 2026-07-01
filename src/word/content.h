#ifndef MUSIC_LYRIC_WORD_CONTENT_H_
#define MUSIC_LYRIC_WORD_CONTENT_H_

#include <cstdint>
#include <string>

#include "word/content.pb.h"

namespace music_lyric_model {
	using namespace lyric;

	/**
	 * Creates a normal word wrapped in a Word.
	 */
	lyric::Word makeWordNormal(const lyric::WordNormal& normal = {});

	/**
	 * Creates a run of spaces wrapped in a Word.
	 */
	lyric::Word makeWordSpace(const lyric::WordSpace& space = {});

	/**
	 * Whether a Word holds a normal word.
	 */
	bool isWordNormal(const lyric::Word& word);

	/**
	 * Whether a Word holds a run of spaces.
	 */
	bool isWordSpace(const lyric::Word& word);

	/**
	 * Rendered text of a word: its content, or padded spaces.
	 */
	std::string getWordText(const lyric::Word& word);

	/**
	 * Time range of a word, null when absent.
	 */
	const lyric::Time* getWordTime(const lyric::Word& word);

	/**
	 * Ruby annotation of a word, null when absent.
	 */
	const lyric::WordAnnotationRuby* getWordRuby(const lyric::Word& word);

	/**
	 * Duration of a word in milliseconds.
	 */
	int64_t getWordDuration(const lyric::Word& word);

	/**
	 * Index of the word active at the given moment, or -1 when none.
	 */
	int getActiveWordIndex(const google::protobuf::RepeatedPtrField<lyric::Word>& words, int64_t ms);

	/**
	 * Word active at the given moment, null when none.
	 */
	const lyric::Word* getActiveWord(const google::protobuf::RepeatedPtrField<lyric::Word>& words, int64_t ms);
} // namespace music_lyric_model

#endif
