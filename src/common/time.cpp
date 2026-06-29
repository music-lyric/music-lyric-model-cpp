#include "common/time.h"

namespace music_lyric_model {
	lyric::Time makeTime() {
		return {};
	}

	int64_t getTimeDuration(const lyric::Time* time) {
		return time ? static_cast<int64_t>(time->end()) - static_cast<int64_t>(time->start()) : 0;
	}
} // namespace music_lyric_model
