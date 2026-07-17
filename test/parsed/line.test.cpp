#include "doctest.h"
#include "music_lyric_model.h"

using namespace music_lyric_model::common;
using namespace music_lyric_model::parsed;

TEST_CASE("asParsedLineNormal and asParsedLineInterlude unwrap the matching variant") {
	lyric::parsed::LineNormal normalInit;
	lyric::common::WordNormal wordInit;
	wordInit.set_content("hi");
	*normalInit.add_words() = makeWordNormal(wordInit);
	const lyric::parsed::Line normal    = makeParsedLineNormal(normalInit);
	const lyric::parsed::Line interlude = makeParsedLineInterlude();

	CHECK(asParsedLineNormal(normal)->words_size() == 1);
	CHECK(asParsedLineNormal(interlude) == nullptr);
	CHECK(asParsedLineInterlude(interlude) != nullptr);
	CHECK(asParsedLineInterlude(normal) == nullptr);
}
