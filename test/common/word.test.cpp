#include <string>
#include <vector>

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

	CHECK(isWordNormal(normal) == true);
	CHECK(isWordSpace(space) == true);
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
	const lyric::common::Word        word   = makeWordNormal(normalInit);
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
	CHECK(getWordText(words.Get(1)) == "  ");
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

TEST_CASE("getActiveWord picks the timed word covering the moment") {
	google::protobuf::RepeatedPtrField<lyric::common::Word> words;

	lyric::common::WordNormal first;
	first.set_content("a");
	first.mutable_time()->set_start(100);
	first.mutable_time()->set_end(200);
	*words.Add() = makeWordNormal(first);

	lyric::common::WordSpace spaceInit;
	spaceInit.set_count(1);
	*words.Add() = makeWordSpace(spaceInit);

	lyric::common::WordNormal second;
	second.set_content("b");
	second.mutable_time()->set_start(200);
	second.mutable_time()->set_end(300);
	*words.Add() = makeWordNormal(second);

	CHECK(getActiveWordIndex(words, 150) == 0);
	CHECK(getActiveWord(words, 250)->normal().content() == "b");
	CHECK(getActiveWordIndex(words, 50) == -1);
	CHECK(getActiveWord(words, 50) == nullptr);
}

TEST_CASE("getWordRubies and getWordAnnotationText read annotation tokens") {
	lyric::common::WordAnnotationContent token;
	token.set_content("kyou");

	lyric::common::WordAnnotationRuby ruby;
	*ruby.add_words() = makeWordAnnotationContent(token);

	lyric::common::WordAnnotation annotation;
	*annotation.add_rubies() = makeWordAnnotationRuby(ruby);

	lyric::common::WordNormal normal;
	normal.set_content("今日");
	*normal.mutable_annotation() = makeWordAnnotation(annotation);
	const lyric::common::Word word = makeWordNormal(normal);

	const auto rubies = getWordRubies(word);
	REQUIRE(rubies.size() == 1);
	CHECK(getWordAnnotationText(*rubies[0]) == "kyou");
	CHECK(getWordRubies(makeWordSpace({})).empty());
}
