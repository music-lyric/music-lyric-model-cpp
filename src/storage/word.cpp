#include "storage/word.h"

#include "common/time.h"

namespace music_lyric_model::storage {
	lyric::storage::Word makeWordNormal(const lyric::storage::WordNormal& normal) {
		lyric::storage::Word word;
		*word.mutable_normal() = normal;
		return word;
	}

	lyric::storage::Word makeWordSpace(const lyric::storage::WordSpace& space) {
		lyric::storage::Word word;
		*word.mutable_space() = space;
		return word;
	}

	lyric::storage::WordAnnotationContent makeWordAnnotationContent(const lyric::storage::WordAnnotationContent& content) {
		return content;
	}

	lyric::storage::WordAnnotationRoman makeWordAnnotationRoman(const lyric::storage::WordAnnotationRoman& roman) {
		return roman;
	}

	lyric::storage::WordAnnotationTranslate makeWordAnnotationTranslate(const lyric::storage::WordAnnotationTranslate& translate) {
		return translate;
	}

	lyric::storage::WordAnnotationRuby makeWordAnnotationRuby(const lyric::storage::WordAnnotationRuby& ruby) {
		return ruby;
	}

	lyric::storage::WordAnnotationUnknown makeWordAnnotationUnknown(const lyric::storage::WordAnnotationUnknown& unknown) {
		return unknown;
	}

	lyric::storage::WordAnnotation makeWordAnnotation(const lyric::storage::WordAnnotation& annotation) {
		return annotation;
	}

	std::string getWordText(const lyric::storage::Word& word) {
		if (word.body_case() == lyric::storage::Word::kNormal) {
			return word.normal().content();
		}
		if (word.body_case() == lyric::storage::Word::kSpace) {
			return std::string(word.space().count(), ' ');
		}
		return {};
	}

	const lyric::common::Time* getWordTime(const lyric::storage::Word& word) {
		if (word.body_case() == lyric::storage::Word::kNormal && word.normal().has_time()) {
			return &word.normal().time();
		}
		return nullptr;
	}

	const lyric::common::Time* getWordTime(const lyric::storage::WordNormal& word) {
		return word.has_time() ? &word.time() : nullptr;
	}

	const lyric::storage::WordAnnotationRuby* getWordRuby(const lyric::storage::Word& word) {
		if (word.body_case() == lyric::storage::Word::kNormal && word.normal().has_annotation() && word.normal().annotation().has_ruby()) {
			return &word.normal().annotation().ruby();
		}
		return nullptr;
	}

	int64_t getWordDuration(const lyric::storage::Word& word) {
		return getTimeDuration(getWordTime(word));
	}

	int64_t getWordDuration(const lyric::storage::WordNormal& word) {
		return getTimeDuration(getWordTime(word));
	}

	int getActiveWordIndex(const google::protobuf::RepeatedPtrField<lyric::storage::Word>& words, int64_t ms) {
		for (int i = 0, len = words.size(); i < len; i++) {
			if (isTimeActive(getWordTime(words.Get(i)), ms)) {
				return i;
			}
		}
		return -1;
	}

	const lyric::storage::Word* getActiveWord(const google::protobuf::RepeatedPtrField<lyric::storage::Word>& words, int64_t ms) {
		const int index = getActiveWordIndex(words, ms);
		return index == -1 ? nullptr : &words.Get(index);
	}

	bool isWordNormal(const lyric::storage::Word& word) {
		return word.body_case() == lyric::storage::Word::kNormal;
	}

	bool isWordSpace(const lyric::storage::Word& word) {
		return word.body_case() == lyric::storage::Word::kSpace;
	}

	const lyric::storage::WordNormal* asWordNormal(const lyric::storage::Word& word) {
		return isWordNormal(word) ? &word.normal() : nullptr;
	}

	const lyric::storage::WordSpace* asWordSpace(const lyric::storage::Word& word) {
		return isWordSpace(word) ? &word.space() : nullptr;
	}
} // namespace music_lyric_model::storage
