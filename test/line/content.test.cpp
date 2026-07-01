#include "line/content.h"

#include <string>
#include <vector>

#include "doctest.h"
#include "line/annotation.h"
#include "word/content.h"

using namespace music_lyric_model;
using namespace lyric;

namespace {
	Line makeNormalLine() {
		WordNormal w1;
		w1.set_content("hello");
		w1.set_language("en");
		WordSpace sp;
		sp.set_count(1);
		WordNormal w2;
		w2.set_content("漢");
		w2.set_language("ja");

		LineNormal   body;
		LineContent* content           = body.mutable_content();
		*content->add_words()          = makeWordNormal(w1);
		*content->add_words()          = makeWordSpace(sp);
		*content->add_words()          = makeWordNormal(w2);
		*content->mutable_annotation() = makeLineAnnotation();

		Time time;
		time.set_start(1000);
		time.set_end(2000);
		return makeLineNormal(body, &time);
	}

	Line makeInterludeLine() {
		Time time;
		time.set_start(2000);
		time.set_end(3000);
		return makeLineInterlude(&time);
	}
} // namespace

TEST_CASE("guards narrow the line body") {
	CHECK(isLineNormal(makeNormalLine()) == true);
	CHECK(isLineInterlude(makeInterludeLine()) == true);
	CHECK(isLineNormal(makeInterludeLine()) == false);
}

TEST_CASE("getLineTime and getLineDuration read across the oneof") {
	const Line normal    = makeNormalLine();
	const Line interlude = makeInterludeLine();
	REQUIRE(getLineTime(normal) != nullptr);
	CHECK(getLineTime(normal)->start() == 1000);
	REQUIRE(getLineTime(interlude) != nullptr);
	CHECK(getLineTime(interlude)->end() == 3000);
	CHECK(getLineDuration(normal) == 1000);
}

TEST_CASE("getLineContent is absent on an interlude") {
	CHECK(getLineContent(makeNormalLine()) != nullptr);
	CHECK(getLineContent(makeInterludeLine()) == nullptr);
}

TEST_CASE("getLineWords is empty for an interlude") {
	const Line normal = makeNormalLine();
	CHECK(getLineWords(normal).size() == 3);
	CHECK(getLineWords(makeInterludeLine()).size() == 0);
}

TEST_CASE("getLineText and getWordsText render the words") {
	const Line normal = makeNormalLine();
	CHECK(getLineText(normal) == "hello 漢");
	CHECK(getLineText(makeInterludeLine()) == "");
	CHECK(getWordsText(getLineWords(normal)) == "hello 漢");
}

TEST_CASE("languages fall back to those of the words") {
	const Line normal = makeNormalLine();
	CHECK(getLineLanguages(normal) == std::vector<std::string>{"en", "ja"});
	CHECK(getWordsLanguages(getLineWords(normal)) == std::vector<std::string>{"en", "ja"});
	CHECK(getLineLanguages(makeInterludeLine()) == std::vector<std::string>{});
}

TEST_CASE("getLineAnnotation is absent on an interlude") {
	CHECK(getLineAnnotation(makeNormalLine()) != nullptr);
	CHECK(getLineAnnotation(makeInterludeLine()) == nullptr);
}
