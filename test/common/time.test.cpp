#include "doctest.h"
#include "music_lyric_model.h"

using namespace music_lyric_model::common;

TEST_CASE("getTimeDuration returns end minus start") {
	const Time time{100, 250};
	CHECK(getTimeDuration(&time) == 150);
	CHECK(getTimeDuration(nullptr) == 0);
}

TEST_CASE("getTimeProgress clamps to 0..1") {
	const Time time{100, 200};
	CHECK(getTimeProgress(&time, 50) == 0);
	CHECK(getTimeProgress(&time, 150) == doctest::Approx(0.5));
	CHECK(getTimeProgress(&time, 250) == 1);
	CHECK(getTimeProgress(nullptr, 150) == 0);
}

TEST_CASE("isTimeActive checks the half-open range") {
	const Time time{100, 200};
	CHECK(isTimeActive(&time, 100) == true);
	CHECK(isTimeActive(&time, 199) == true);
	CHECK(isTimeActive(&time, 200) == false);
	CHECK(isTimeActive(nullptr, 150) == false);
}
