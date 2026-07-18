#include "common/word.h"

#include <unordered_set>
#include <utility>

#include "common/time.h"

namespace music_lyric_model::common {
	Word makeWordNormal(WordNormal normal) {
		return Word{std::move(normal)};
	}

	Word makeWordSpace(WordSpace space) {
		return Word{std::move(space)};
	}

	WordAnnotationContent makeWordAnnotationContent(WordAnnotationContent init) {
		return init;
	}

	WordAnnotationRoman makeWordAnnotationRoman(WordAnnotationRoman init) {
		return init;
	}

	WordAnnotationTranslation makeWordAnnotationTranslation(WordAnnotationTranslation init) {
		return init;
	}

	WordAnnotationRuby makeWordAnnotationRuby(WordAnnotationRuby init) {
		return init;
	}

	WordAnnotation makeWordAnnotation(WordAnnotation init) {
		return init;
	}

	bool isWordNormal(const Word& word) {
		return std::holds_alternative<WordNormal>(word);
	}

	bool isWordSpace(const Word& word) {
		return std::holds_alternative<WordSpace>(word);
	}

	const WordNormal* asWordNormal(const Word& word) {
		return std::get_if<WordNormal>(&word);
	}

	const WordSpace* asWordSpace(const Word& word) {
		return std::get_if<WordSpace>(&word);
	}

	std::string getWordText(const Word& word) {
		if (const WordNormal* normal = asWordNormal(word)) {
			return normal->content;
		}
		if (const WordSpace* space = asWordSpace(word)) {
			return std::string(space->count, ' ');
		}
		return {};
	}

	std::string getWordsText(const std::vector<Word>& words) {
		std::string result;
		for (const auto& word : words) {
			result += getWordText(word);
		}
		return result;
	}

	std::vector<std::string> getWordsLanguages(const std::vector<Word>& words) {
		std::vector<std::string>        result;
		std::unordered_set<std::string> seen;
		for (const auto& word : words) {
			const WordNormal* normal = asWordNormal(word);
			if (!normal || !normal->language.has_value()) {
				continue;
			}
			const std::string& tag = *normal->language;
			if (seen.insert(tag).second) {
				result.push_back(tag);
			}
		}
		return result;
	}

	const Time* getWordTime(const Word& word) {
		const WordNormal* normal = asWordNormal(word);
		return normal ? getWordTime(*normal) : nullptr;
	}

	const Time* getWordTime(const WordNormal& word) {
		return word.time.has_value() ? &*word.time : nullptr;
	}

	std::vector<const WordAnnotationRuby*> getWordRubies(const Word& word) {
		std::vector<const WordAnnotationRuby*> result;
		const WordNormal*                      normal = asWordNormal(word);
		if (!normal || !normal->annotation.has_value()) {
			return result;
		}
		for (const auto& ruby : normal->annotation->rubies) {
			result.push_back(&ruby);
		}
		return result;
	}

	int64_t getWordDuration(const Word& word) {
		return getTimeDuration(getWordTime(word));
	}

	int64_t getWordDuration(const WordNormal& word) {
		return getTimeDuration(getWordTime(word));
	}

	int getActiveWordIndex(const std::vector<Word>& words, int64_t ms) {
		for (int i = 0, len = static_cast<int>(words.size()); i < len; i++) {
			if (isTimeActive(getWordTime(words[static_cast<size_t>(i)]), ms)) {
				return i;
			}
		}
		return -1;
	}

	const Word* getActiveWord(const std::vector<Word>& words, int64_t ms) {
		const int index = getActiveWordIndex(words, ms);
		return index == -1 ? nullptr : &words[static_cast<size_t>(index)];
	}
} // namespace music_lyric_model::common
