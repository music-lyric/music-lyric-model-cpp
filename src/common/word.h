#ifndef MUSIC_LYRIC_MODEL_COMMON_WORD_H_
#define MUSIC_LYRIC_MODEL_COMMON_WORD_H_

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "time.h"
#include "unknown.h"

namespace music_lyric_model::common {
	/**
	 * One token of a word annotation.
	 */
	struct WordAnnotationContent {
		std::optional<Time> time;
		std::string         content;
	};

	/**
	 * A word-level translation.
	 */
	struct WordAnnotationTranslation {
		std::optional<std::string> language;
		std::string                content;
	};

	/**
	 * A word-level romanization.
	 */
	struct WordAnnotationRoman {
		std::optional<Time>                 time;
		std::optional<std::string>          language;
		std::vector<WordAnnotationContent>  words;
	};

	/**
	 * A word-level ruby annotation.
	 */
	struct WordAnnotationRuby {
		std::optional<Time>                time;
		std::optional<std::string>         language;
		std::vector<WordAnnotationContent> words;
		bool                               phraseStart = false;
	};

	/**
	 * The per-word annotation container.
	 */
	struct WordAnnotation {
		std::vector<Unknown>                   unknowns;
		std::vector<WordAnnotationRuby>        rubies;
		std::vector<WordAnnotationRoman>       romans;
		std::vector<WordAnnotationTranslation> translations;
	};

	/**
	 * A normal lyric word.
	 */
	struct WordNormal {
		std::unordered_map<std::string, std::string> extra;
		std::optional<Time>                          time;
		std::string                                  content;
		std::optional<std::string>                   language;
		std::optional<WordAnnotation>                annotation;
		bool                                         stress = false;
	};

	/**
	 * A run of consecutive space characters.
	 */
	struct WordSpace {
		uint32_t count = 0;
	};

	/**
	 * A word token; exactly one body variant must be set.
	 */
	using Word = std::variant<WordNormal, WordSpace>;

	/**
	 * Creates a normal word wrapped in a Word.
	 */
	Word makeWordNormal(WordNormal normal = {});

	/**
	 * Creates a run of spaces wrapped in a Word.
	 */
	Word makeWordSpace(WordSpace space = {});

	/**
	 * Whether a Word holds a normal word.
	 */
	bool isWordNormal(const Word& word);

	/**
	 * Whether a Word holds a run of spaces.
	 */
	bool isWordSpace(const Word& word);

	/**
	 * Normal word held by a Word, null when it holds something else.
	 */
	const WordNormal* asWordNormal(const Word& word);

	/**
	 * Space run held by a Word, null when it holds something else.
	 */
	const WordSpace* asWordSpace(const Word& word);

	/**
	 * Rendered text of a word: its content, or padded spaces.
	 */
	std::string getWordText(const Word& word);

	/**
	 * Text joined from words in order.
	 */
	std::string getWordsText(const std::vector<Word>& words);

	/**
	 * Distinct language tags among a list of words, in first-seen order.
	 */
	std::vector<std::string> getWordsLanguages(const std::vector<Word>& words);

	/**
	 * Time range of a word, null when absent.
	 */
	const Time* getWordTime(const Word& word);

	/**
	 * Time range of a bare normal word, null when absent.
	 */
	const Time* getWordTime(const WordNormal& word);

	/**
	 * Ruby annotations of a normal word.
	 */
	std::vector<const WordAnnotationRuby*> getWordRubies(const Word& word);

	/**
	 * Duration of a word in milliseconds.
	 */
	int64_t getWordDuration(const Word& word);

	/**
	 * Duration of a bare normal word in milliseconds.
	 */
	int64_t getWordDuration(const WordNormal& word);

	/**
	 * Index of the word active at the given moment, or -1 when none.
	 */
	int getActiveWordIndex(const std::vector<Word>& words, int64_t ms);

	/**
	 * Word active at the given moment, null when none.
	 */
	const Word* getActiveWord(const std::vector<Word>& words, int64_t ms);

	/**
	 * Text joined from every annotation token.
	 */
	template <typename T>
	std::string getWordAnnotationText(const T& item) {
		std::string result;
		for (const auto& word : item.words) {
			result += word.content;
		}
		return result;
	}
} // namespace music_lyric_model::common

#endif
