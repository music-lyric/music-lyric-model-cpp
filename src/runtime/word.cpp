#include "runtime/word.h"

#include "common/time.h"

namespace music_lyric_model {
	lyric::runtime::Word makeWordNormal(const lyric::runtime::WordNormal& normal) {
		lyric::runtime::Word word;
		*word.mutable_normal() = normal;
		return word;
	}

	lyric::runtime::Word makeWordSpace(const lyric::runtime::WordSpace& space) {
		lyric::runtime::Word word;
		*word.mutable_space() = space;
		return word;
	}

	lyric::runtime::WordAnnotationContent makeWordAnnotationContent(const lyric::runtime::WordAnnotationContent& content) {
		return content;
	}

	lyric::runtime::WordAnnotationRoman makeWordAnnotationRoman(const lyric::runtime::WordAnnotationRoman& roman) {
		return roman;
	}

	lyric::runtime::WordAnnotationTranslate makeWordAnnotationTranslate(const lyric::runtime::WordAnnotationTranslate& translate) {
		return translate;
	}

	lyric::runtime::WordAnnotationRuby makeWordAnnotationRuby(const lyric::runtime::WordAnnotationRuby& ruby) {
		return ruby;
	}

	lyric::runtime::WordAnnotationUnknown makeWordAnnotationUnknown(const lyric::runtime::WordAnnotationUnknown& unknown) {
		return unknown;
	}

	lyric::runtime::WordAnnotation makeWordAnnotation(const lyric::runtime::WordAnnotation& annotation) {
		return annotation;
	}

	bool isWordNormal(const lyric::runtime::Word& word) {
		return word.body_case() == lyric::runtime::Word::kNormal;
	}

	bool isWordSpace(const lyric::runtime::Word& word) {
		return word.body_case() == lyric::runtime::Word::kSpace;
	}

	const lyric::runtime::WordNormal* asWordNormal(const lyric::runtime::Word& word) {
		return isWordNormal(word) ? &word.normal() : nullptr;
	}

	const lyric::runtime::WordSpace* asWordSpace(const lyric::runtime::Word& word) {
		return isWordSpace(word) ? &word.space() : nullptr;
	}

	std::string getWordText(const lyric::runtime::Word& word) {
		if (word.body_case() == lyric::runtime::Word::kNormal) {
			return word.normal().content();
		}
		if (word.body_case() == lyric::runtime::Word::kSpace) {
			return std::string(word.space().count(), ' ');
		}
		return {};
	}

	const lyric::common::Time* getWordTime(const lyric::runtime::Word& word) {
		if (word.body_case() == lyric::runtime::Word::kNormal && word.normal().has_time()) {
			return &word.normal().time();
		}
		return nullptr;
	}

	const lyric::common::Time* getWordTime(const lyric::runtime::WordNormal& word) {
		return word.has_time() ? &word.time() : nullptr;
	}

	const lyric::runtime::WordAnnotationRuby* getWordRuby(const lyric::runtime::Word& word) {
		if (word.body_case() == lyric::runtime::Word::kNormal && word.normal().has_annotation() && word.normal().annotation().has_ruby()) {
			return &word.normal().annotation().ruby();
		}
		return nullptr;
	}

	int64_t getWordDuration(const lyric::runtime::Word& word) {
		return getTimeDuration(getWordTime(word));
	}

	int64_t getWordDuration(const lyric::runtime::WordNormal& word) {
		return getTimeDuration(getWordTime(word));
	}

	int getActiveWordIndex(const google::protobuf::RepeatedPtrField<lyric::runtime::Word>& words, int64_t ms) {
		for (int i = 0, len = words.size(); i < len; i++) {
			if (isTimeActive(getWordTime(words.Get(i)), ms)) {
				return i;
			}
		}
		return -1;
	}

	const lyric::runtime::Word* getActiveWord(const google::protobuf::RepeatedPtrField<lyric::runtime::Word>& words, int64_t ms) {
		const int index = getActiveWordIndex(words, ms);
		return index == -1 ? nullptr : &words.Get(index);
	}
} // namespace music_lyric_model
