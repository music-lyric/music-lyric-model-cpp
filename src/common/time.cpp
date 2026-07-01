#include "common/time.h"

namespace music_lyric_model {
	lyric::Time makeTime() {
		return {};
	}

	int64_t getTimeDuration(const lyric::Time* time) {
		return time ? static_cast<int64_t>(time->end()) - static_cast<int64_t>(time->start()) : 0;
	}

	double getTimeProgress(const lyric::Time* time, int64_t ms) {
		const int64_t duration = getTimeDuration(time);
		if (!time || duration <= 0) {
			return 0;
		}
		if (ms <= static_cast<int64_t>(time->start())) {
			return 0;
		}
		if (ms >= static_cast<int64_t>(time->end())) {
			return 1;
		}
		return static_cast<double>(ms - static_cast<int64_t>(time->start())) / static_cast<double>(duration);
	}

	bool isTimeActive(const lyric::Time* time, int64_t ms) {
		return time ? ms >= static_cast<int64_t>(time->start()) && ms < static_cast<int64_t>(time->end()) : false;
	}
} // namespace music_lyric_model
