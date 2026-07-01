#ifndef MUSIC_LYRIC_COMMON_TIME_H_
#define MUSIC_LYRIC_COMMON_TIME_H_

#include <cstdint>

#include "common/time.pb.h"

namespace music_lyric_model {
	using namespace lyric;

	/**
	 * Creates a Time.
	 */
	lyric::Time makeTime();

	/**
	 * Duration of a time range in milliseconds, zero when absent.
	 */
	int64_t getTimeDuration(const lyric::Time* time);

	/**
	 * Progress within a time range at the given moment, clamped to 0..1.
	 */
	double getTimeProgress(const lyric::Time* time, int64_t ms);

	/**
	 * Whether the time range is active at the given moment in milliseconds.
	 */
	bool isTimeActive(const lyric::Time* time, int64_t ms);
} // namespace music_lyric_model

#endif
