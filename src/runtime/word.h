#ifndef MUSIC_LYRIC_RUNTIME_WORD_H_
#define MUSIC_LYRIC_RUNTIME_WORD_H_

#include <cstdint>
#include <string>

#include "common/time.pb.h"
#include "runtime/word/annotation.pb.h"
#include "runtime/word/content.pb.h"

namespace music_lyric_model::runtime {
	using namespace lyric::common;
	using namespace lyric::runtime;

	/**
	 * Creates a normal word wrapped in a Word.
	 */
	lyric::runtime::Word makeWordNormal(const lyric::runtime::WordNormal& normal = {});

	/**
	 * Creates a run of spaces wrapped in a Word.
	 */
	lyric::runtime::Word makeWordSpace(const lyric::runtime::WordSpace& space = {});

	/**
	 * Creates a WordAnnotationContent, one token of a word annotation.
	 */
	lyric::runtime::WordAnnotationContent makeWordAnnotationContent(const lyric::runtime::WordAnnotationContent& content = {});

	/**
	 * Creates a WordAnnotationRoman.
	 */
	lyric::runtime::WordAnnotationRoman makeWordAnnotationRoman(const lyric::runtime::WordAnnotationRoman& roman = {});

	/**
	 * Creates a WordAnnotationTranslate.
	 */
	lyric::runtime::WordAnnotationTranslate makeWordAnnotationTranslate(const lyric::runtime::WordAnnotationTranslate& translate = {});

	/**
	 * Creates a WordAnnotationRuby.
	 */
	lyric::runtime::WordAnnotationRuby makeWordAnnotationRuby(const lyric::runtime::WordAnnotationRuby& ruby = {});

	/**
	 * Creates a WordAnnotationUnknown.
	 */
	lyric::runtime::WordAnnotationUnknown makeWordAnnotationUnknown(const lyric::runtime::WordAnnotationUnknown& unknown = {});

	/**
	 * Creates a WordAnnotation, the per-word annotation container.
	 */
	lyric::runtime::WordAnnotation makeWordAnnotation(const lyric::runtime::WordAnnotation& annotation = {});

	/**
	 * Whether a Word holds a normal word.
	 */
	bool isWordNormal(const lyric::runtime::Word& word);

	/**
	 * Whether a Word holds a run of spaces.
	 */
	bool isWordSpace(const lyric::runtime::Word& word);

	/**
	 * Normal word held by a Word, null when it holds something else.
	 */
	const lyric::runtime::WordNormal* asWordNormal(const lyric::runtime::Word& word);

	/**
	 * Space run held by a Word, null when it holds something else.
	 */
	const lyric::runtime::WordSpace* asWordSpace(const lyric::runtime::Word& word);

	/**
	 * Rendered text of a word: its content, or padded spaces.
	 */
	std::string getWordText(const lyric::runtime::Word& word);

	/**
	 * Time range of a word, null when absent.
	 */
	const lyric::common::Time* getWordTime(const lyric::runtime::Word& word);

	/**
	 * Time range of a bare normal word, null when absent.
	 */
	const lyric::common::Time* getWordTime(const lyric::runtime::WordNormal& word);

	/**
	 * Ruby annotation of a word, null when absent.
	 */
	const lyric::runtime::WordAnnotationRuby* getWordRuby(const lyric::runtime::Word& word);

	/**
	 * Duration of a word in milliseconds.
	 */
	int64_t getWordDuration(const lyric::runtime::Word& word);

	/**
	 * Duration of a bare normal word in milliseconds.
	 */
	int64_t getWordDuration(const lyric::runtime::WordNormal& word);

	/**
	 * Index of the word active at the given moment, or -1 when none.
	 */
	int getActiveWordIndex(const google::protobuf::RepeatedPtrField<lyric::runtime::Word>& words, int64_t ms);

	/**
	 * Word active at the given moment, null when none.
	 */
	const lyric::runtime::Word* getActiveWord(const google::protobuf::RepeatedPtrField<lyric::runtime::Word>& words, int64_t ms);

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
} // namespace music_lyric_model::runtime

#endif
