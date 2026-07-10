#include "doctest.h"
#include "music_lyric_model.h"

using namespace lyric::common;
using namespace lyric::storage;

TEST_CASE("storage part maker preserves initial values") {
	Part init;
	init.set_type(PART_TYPE_CHORUS);
	init.set_label("Chorus");
	const Part part = music_lyric_model::storage::makePart(init);
	CHECK(part.type() == PART_TYPE_CHORUS);
	REQUIRE(part.has_label() == true);
	CHECK(part.label() == "Chorus");
}
