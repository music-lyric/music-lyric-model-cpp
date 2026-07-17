#include "doctest.h"
#include "music_lyric_model.h"

using namespace music_lyric_model::common;

TEST_CASE("asWordNormal and asWordSpace unwrap the matching variant") {
	lyric::common::WordNormal normalInit;
	normalInit.set_content("hi");
	const lyric::common::Word normal = makeWordNormal(normalInit);

	lyric::common::WordSpace spaceInit;
	spaceInit.set_count(2);
	const lyric::common::Word space = makeWordSpace(spaceInit);

	CHECK(asWordNormal(normal)->content() == "hi");
	CHECK(asWordNormal(space) == nullptr);
	CHECK(asWordSpace(space)->count() == 2);
	CHECK(asWordSpace(normal) == nullptr);
}

TEST_CASE("getWordTime and getWordDuration accept a Word or a bare WordNormal") {
	lyric::common::WordNormal normalInit;
	normalInit.set_content("hi");
	normalInit.mutable_time()->set_start(1000);
	normalInit.mutable_time()->set_end(1500);
	const lyric::common::Word       word   = makeWordNormal(normalInit);
	const lyric::common::WordNormal* normal = asWordNormal(word);

	CHECK(getWordTime(word)->start() == 1000);
	CHECK(getWordTime(*normal)->start() == 1000);
	CHECK(getWordDuration(word) == 500);
	CHECK(getWordDuration(*normal) == 500);
}

TEST_CASE("getWordTime is null and getWordDuration zero for a space") {
	lyric::common::WordSpace spaceInit;
	spaceInit.set_count(1);
	const lyric::common::Word space = makeWordSpace(spaceInit);

	CHECK(getWordTime(space) == nullptr);
	CHECK(getWordDuration(space) == 0);
}

TEST_CASE("getWordsText joins rendered word text") {
	google::protobuf::RepeatedPtrField<lyric::common::Word> words;

	lyric::common::WordNormal normalInit;
	normalInit.set_content("hi");
	*words.Add() = makeWordNormal(normalInit);

	lyric::common::WordSpace spaceInit;
	spaceInit.set_count(2);
	*words.Add() = makeWordSpace(spaceInit);

	lyric::common::WordNormal tailInit;
	tailInit.set_content("!");
	*words.Add() = makeWordNormal(tailInit);

	CHECK(getWordsText(words) == "hi  !");
}

TEST_CASE("getWordsLanguages returns distinct tags in first-seen order") {
	google::protobuf::RepeatedPtrField<lyric::common::Word> words;

	lyric::common::WordNormal a;
	a.set_content("a");
	a.set_language("ja");
	*words.Add() = makeWordNormal(a);

	lyric::common::WordSpace spaceInit;
	spaceInit.set_count(1);
	*words.Add() = makeWordSpace(spaceInit);

	lyric::common::WordNormal b;
	b.set_content("b");
	b.set_language("en");
	*words.Add() = makeWordNormal(b);

	lyric::common::WordNormal c;
	c.set_content("c");
	c.set_language("ja");
	*words.Add() = makeWordNormal(c);

	const std::vector<std::string> tags = getWordsLanguages(words);
	REQUIRE(tags.size() == 2);
	CHECK(tags[0] == "ja");
	CHECK(tags[1] == "en");
}
