#include "runtime/part.h"

#include "doctest.h"

using namespace music_lyric_model;
using namespace lyric::common;
using namespace lyric::runtime;

TEST_CASE("makePart returns a default part") {
	const Part part = makePart();
	CHECK(part.type() == PART_TYPE_UNSPECIFIED);
	CHECK(part.has_label() == false);
}

TEST_CASE("makePart preserves a type and an optional label") {
	Part init;
	init.set_type(PART_TYPE_CHORUS);
	init.set_label("Chorus");
	const Part part = makePart(init);
	CHECK(part.type() == PART_TYPE_CHORUS);
	REQUIRE(part.has_label() == true);
	CHECK(part.label() == "Chorus");
}
