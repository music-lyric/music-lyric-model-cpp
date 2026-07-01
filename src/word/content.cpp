#include "word/content.h"

#include "common/time.h"

namespace music_lyric_model {
	lyric::Word makeWordNormal(const lyric::WordNormal& normal) {
		lyric::Word word;
		*word.mutable_normal() = normal;
		return word;
	}

	lyric::Word makeWordSpace(const lyric::WordSpace& space) {
		lyric::Word word;
		*word.mutable_space() = space;
		return word;
	}

	bool isWordNormal(const lyric::Word& word) {
		return word.body_case() == lyric::Word::kNormal;
	}

	bool isWordSpace(const lyric::Word& word) {
		return word.body_case() == lyric::Word::kSpace;
	}

	std::string getWordText(const lyric::Word& word) {
		if (word.body_case() == lyric::Word::kNormal) {
			return word.normal().content();
		}
		if (word.body_case() == lyric::Word::kSpace) {
			return std::string(word.space().count(), ' ');
		}
		return {};
	}

	const lyric::Time* getWordTime(const lyric::Word& word) {
		if (word.body_case() == lyric::Word::kNormal && word.normal().has_time()) {
			return &word.normal().time();
		}
		return nullptr;
	}

	const lyric::WordAnnotationRuby* getWordRuby(const lyric::Word& word) {
		if (word.body_case() == lyric::Word::kNormal && word.normal().has_annotation() && word.normal().annotation().has_ruby()) {
			return &word.normal().annotation().ruby();
		}
		return nullptr;
	}

	int64_t getWordDuration(const lyric::Word& word) {
		return getTimeDuration(getWordTime(word));
	}

	int getActiveWordIndex(const google::protobuf::RepeatedPtrField<lyric::Word>& words, int64_t ms) {
		for (int i = 0, len = words.size(); i < len; i++) {
			if (isTimeActive(getWordTime(words.Get(i)), ms)) {
				return i;
			}
		}
		return -1;
	}

	const lyric::Word* getActiveWord(const google::protobuf::RepeatedPtrField<lyric::Word>& words, int64_t ms) {
		const int index = getActiveWordIndex(words, ms);
		return index == -1 ? nullptr : &words.Get(index);
	}
} // namespace music_lyric_model
