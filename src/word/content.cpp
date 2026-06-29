#include "word/content.h"

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
} // namespace music_lyric_model
