#include "common/word.h"

#include <unordered_set>

#include "common/time.h"

namespace music_lyric_model::common {
	lyric::common::Word makeWordNormal(const lyric::common::WordNormal& normal) {
		lyric::common::Word word;
		*word.mutable_normal() = normal;
		return word;
	}

	lyric::common::Word makeWordSpace(const lyric::common::WordSpace& space) {
		lyric::common::Word word;
		*word.mutable_space() = space;
		return word;
	}

	lyric::common::WordAnnotationContent makeWordAnnotationContent(const lyric::common::WordAnnotationContent& content) {
		return content;
	}

	lyric::common::WordAnnotationRoman makeWordAnnotationRoman(const lyric::common::WordAnnotationRoman& roman) {
		return roman;
	}

	lyric::common::WordAnnotationTranslation makeWordAnnotationTranslation(const lyric::common::WordAnnotationTranslation& translation) {
		return translation;
	}

	lyric::common::WordAnnotationRuby makeWordAnnotationRuby(const lyric::common::WordAnnotationRuby& ruby) {
		return ruby;
	}

	lyric::common::WordAnnotation makeWordAnnotation(const lyric::common::WordAnnotation& annotation) {
		return annotation;
	}

	bool isWordNormal(const lyric::common::Word& word) {
		return word.body_case() == lyric::common::Word::kNormal;
	}

	bool isWordSpace(const lyric::common::Word& word) {
		return word.body_case() == lyric::common::Word::kSpace;
	}

	const lyric::common::WordNormal* asWordNormal(const lyric::common::Word& word) {
		return isWordNormal(word) ? &word.normal() : nullptr;
	}

	const lyric::common::WordSpace* asWordSpace(const lyric::common::Word& word) {
		return isWordSpace(word) ? &word.space() : nullptr;
	}

	std::string getWordText(const lyric::common::Word& word) {
		if (word.body_case() == lyric::common::Word::kNormal) {
			return word.normal().content();
		}
		if (word.body_case() == lyric::common::Word::kSpace) {
			return std::string(word.space().count(), ' ');
		}
		return {};
	}

	std::string getWordsText(const google::protobuf::RepeatedPtrField<lyric::common::Word>& words) {
		std::string result;
		for (const auto& word : words) {
			result += getWordText(word);
		}
		return result;
	}

	std::vector<std::string> getWordsLanguages(const google::protobuf::RepeatedPtrField<lyric::common::Word>& words) {
		std::vector<std::string>        result;
		std::unordered_set<std::string> seen;
		for (const auto& word : words) {
			if (word.body_case() != lyric::common::Word::kNormal || !word.normal().has_language()) {
				continue;
			}
			const std::string& tag = word.normal().language();
			if (seen.insert(tag).second) {
				result.push_back(tag);
			}
		}
		return result;
	}

	const lyric::common::Time* getWordTime(const lyric::common::Word& word) {
		if (word.body_case() == lyric::common::Word::kNormal && word.normal().has_time()) {
			return &word.normal().time();
		}
		return nullptr;
	}

	const lyric::common::Time* getWordTime(const lyric::common::WordNormal& word) {
		return word.has_time() ? &word.time() : nullptr;
	}

	std::vector<const lyric::common::WordAnnotationRuby*> getWordRubies(const lyric::common::Word& word) {
		std::vector<const lyric::common::WordAnnotationRuby*> result;
		if (word.body_case() != lyric::common::Word::kNormal || !word.normal().has_annotation()) {
			return result;
		}
		for (const auto& ruby : word.normal().annotation().rubies()) {
			result.push_back(&ruby);
		}
		return result;
	}

	int64_t getWordDuration(const lyric::common::Word& word) {
		return getTimeDuration(getWordTime(word));
	}

	int64_t getWordDuration(const lyric::common::WordNormal& word) {
		return getTimeDuration(getWordTime(word));
	}

	int getActiveWordIndex(const google::protobuf::RepeatedPtrField<lyric::common::Word>& words, int64_t ms) {
		for (int i = 0, len = words.size(); i < len; i++) {
			if (isTimeActive(getWordTime(words.Get(i)), ms)) {
				return i;
			}
		}
		return -1;
	}

	const lyric::common::Word* getActiveWord(const google::protobuf::RepeatedPtrField<lyric::common::Word>& words, int64_t ms) {
		const int index = getActiveWordIndex(words, ms);
		return index == -1 ? nullptr : &words.Get(index);
	}
} // namespace music_lyric_model::common
