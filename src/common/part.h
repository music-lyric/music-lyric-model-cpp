#ifndef MUSIC_LYRIC_COMMON_PART_H_
#define MUSIC_LYRIC_COMMON_PART_H_

#include "common/part.pb.h"

namespace music_lyric_model::common {
	/**
	 * Creates a Part, a line's structural section.
	 */
	lyric::common::Part makePart(const lyric::common::Part& part = {});
} // namespace music_lyric_model::common

#endif
