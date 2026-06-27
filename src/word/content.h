#ifndef MUSIC_LYRIC_WORD_CONTENT_H_
#define MUSIC_LYRIC_WORD_CONTENT_H_

#include "word/content.pb.h"

namespace music_lyric_model {
	using namespace lyric;

	/**
	 * Creates a normal word wrapped in a Word.
	 */
	lyric::Word makeWordNormal(const lyric::WordNormal &normal = {});

	/**
	 * Creates a run of spaces wrapped in a Word.
	 */
	lyric::Word makeWordSpace(const lyric::WordSpace &space = {});
}

#endif
