#include "common/common.h"
#include "doctest.h"

using namespace music_lyric_model;
using namespace lyric::common;

TEST_CASE("makeTime preserves initial values") {
	Time init;
	init.set_start(100);
	init.set_end(200);
	const Time time = makeTime(init);
	CHECK(time.start() == 100);
	CHECK(time.end() == 200);
}

TEST_CASE("getTimeDuration returns end minus start") {
	Time time;
	time.set_start(100);
	time.set_end(250);
	CHECK(getTimeDuration(&time) == 150);
	CHECK(getTimeDuration(nullptr) == 0);
}

TEST_CASE("getTimeProgress clamps to 0..1") {
	Time time;
	time.set_start(100);
	time.set_end(200);
	CHECK(getTimeProgress(&time, 50) == 0);
	CHECK(getTimeProgress(&time, 150) == doctest::Approx(0.5));
	CHECK(getTimeProgress(&time, 250) == 1);
	CHECK(getTimeProgress(nullptr, 150) == 0);
}

TEST_CASE("isTimeActive checks the half-open range") {
	Time time;
	time.set_start(100);
	time.set_end(200);
	CHECK(isTimeActive(&time, 100) == true);
	CHECK(isTimeActive(&time, 199) == true);
	CHECK(isTimeActive(&time, 200) == false);
	CHECK(isTimeActive(nullptr, 150) == false);
}
