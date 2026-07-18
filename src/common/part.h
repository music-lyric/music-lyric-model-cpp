#ifndef MUSIC_LYRIC_MODEL_COMMON_PART_H_
#define MUSIC_LYRIC_MODEL_COMMON_PART_H_

#include "model/common/part.gen.h"

namespace music_lyric_model::common {
	/**
	 * Creates a Part, a line's structural section.
	 */
	Part makePart(Part init = {});
} // namespace music_lyric_model::common

#endif
