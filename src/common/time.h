#ifndef MUSIC_LYRIC_MODEL_COMMON_TIME_H_
#define MUSIC_LYRIC_MODEL_COMMON_TIME_H_

#include <cstdint>

namespace music_lyric_model::common {
	/**
	 * A half-open time range in milliseconds.
	 */
	struct Time {
		uint32_t start = 0;
		uint32_t end   = 0;
	};

	/**
	 * Duration of a time range in milliseconds, zero when absent.
	 */
	int64_t getTimeDuration(const Time* time);

	/**
	 * Progress within a time range at the given moment, clamped to 0..1.
	 */
	double getTimeProgress(const Time* time, int64_t ms);

	/**
	 * Whether the time range is active at the given moment in milliseconds.
	 */
	bool isTimeActive(const Time* time, int64_t ms);
} // namespace music_lyric_model::common

#endif
