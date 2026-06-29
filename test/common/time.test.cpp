#include "common/time.h"

#include "doctest.h"

using namespace music_lyric_model;
using namespace lyric;

TEST_CASE("makeTime returns a default time") {
	Time time = makeTime();
	CHECK(time.start() == 0);
	CHECK(time.end() == 0);
}

TEST_CASE("getTimeDuration returns end minus start") {
	Time time;
	time.set_start(100);
	time.set_end(250);
	CHECK(getTimeDuration(&time) == 150);
	CHECK(getTimeDuration(nullptr) == 0);
}
