#include "word/content.h"

namespace music_lyric_model {
	lyric::Word makeWordNormal(const lyric::WordNormal &normal) {
		lyric::Word word;
		*word.mutable_normal() = normal;
		return word;
	}

	lyric::Word makeWordSpace(const lyric::WordSpace &space) {
		lyric::Word word;
		*word.mutable_space() = space;
		return word;
	}
}
