#ifndef MUSIC_LYRIC_LINE_CONTENT_H_
#define MUSIC_LYRIC_LINE_CONTENT_H_

#include "line/content.pb.h"

namespace music_lyric_model {
	using namespace lyric;

	/**
	 * Creates a normal line wrapped in a Line.
	 */
	lyric::Line makeLineNormal(const lyric::LineNormal &normal = {});

	/**
	 * Creates a LineBackground, a background line attached to a normal line.
	 */
	lyric::LineBackground makeLineBackground();

	/**
	 * Creates an interlude wrapped in a Line.
	 */
	lyric::Line makeLineInterlude(const lyric::LineInterlude &interlude = {});
}

#endif
