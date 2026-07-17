#ifndef MUSIC_LYRIC_MODEL_COMMON_H_
#define MUSIC_LYRIC_MODEL_COMMON_H_

#include "agent.h"
#include "line.h"
#include "meta.h"
#include "part.h"
#include "time.h"
#include "unknown.h"
#include "word.h"

namespace music_lyric_model::common {
	/**
	 * Declared timing precision for a lyric.
	 */
	enum class Timing {
		Unspecified = 0,
		None        = 1,
		Line        = 2,
		Word        = 3,
	};
} // namespace music_lyric_model::common

#endif
