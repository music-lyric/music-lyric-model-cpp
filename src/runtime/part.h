#ifndef MUSIC_LYRIC_RUNTIME_PART_H_
#define MUSIC_LYRIC_RUNTIME_PART_H_

#include "runtime/part/part.pb.h"

namespace music_lyric_model {
	using namespace lyric::common;
	using namespace lyric::runtime;

	/**
	 * Creates a Part, a line's structural section.
	 */
	lyric::runtime::Part makePart(const lyric::runtime::Part& part = {});
} // namespace music_lyric_model

#endif
