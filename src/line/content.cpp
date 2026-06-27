#include "line/content.h"

namespace music_lyric_model {
	lyric::Line makeLineNormal(const lyric::LineNormal &normal) {
		lyric::Line line;
		*line.mutable_normal() = normal;
		return line;
	}

	lyric::LineBackground makeLineBackground() {
		return {};
	}

	lyric::Line makeLineInterlude(const lyric::LineInterlude &interlude) {
		lyric::Line line;
		*line.mutable_interlude() = interlude;
		return line;
	}
}
