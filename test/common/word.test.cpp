#include <string>
#include <vector>

#include "doctest.h"
#include "music_lyric_model.h"

using namespace music_lyric_model::common;

TEST_CASE("asWordNormal and asWordSpace unwrap the matching variant") {
	WordNormal normalInit;
	normalInit.content = "hi";
	const Word normal  = makeWordNormal(normalInit);

	WordSpace spaceInit;
	spaceInit.count  = 2;
	const Word space = makeWordSpace(spaceInit);

	CHECK(isWordNormal(normal) == true);
	CHECK(isWordSpace(space) == true);
	CHECK(asWordNormal(normal)->content == "hi");
	CHECK(asWordNormal(space) == nullptr);
	CHECK(asWordSpace(space)->count == 2);
	CHECK(asWordSpace(normal) == nullptr);
}

TEST_CASE("getWordTime and getWordDuration accept a Word or a bare WordNormal") {
	WordNormal normalInit;
	normalInit.content       = "hi";
	normalInit.time          = Time{1000, 1500};
	const Word        word   = makeWordNormal(normalInit);
	const WordNormal* normal = asWordNormal(word);

	CHECK(getWordTime(word)->start == 1000);
	CHECK(getWordTime(*normal)->start == 1000);
	CHECK(getWordDuration(word) == 500);
	CHECK(getWordDuration(*normal) == 500);
}

TEST_CASE("getWordTime is null and getWordDuration zero for a space") {
	WordSpace spaceInit;
	spaceInit.count  = 1;
	const Word space = makeWordSpace(spaceInit);

	CHECK(getWordTime(space) == nullptr);
	CHECK(getWordDuration(space) == 0);
}

TEST_CASE("getWordsText joins rendered word text") {
	WordNormal a;
	a.content = "hi";
	WordSpace space;
	space.count = 2;
	WordNormal b;
	b.content                     = "!";
	const std::vector<Word> words = {
		makeWordNormal(a),
		makeWordSpace(space),
		makeWordNormal(b),
	};

	CHECK(getWordsText(words) == "hi  !");
	CHECK(getWordText(words[1]) == "  ");
}

TEST_CASE("getWordsLanguages returns distinct tags in first-seen order") {
	WordNormal a;
	a.content  = "a";
	a.language = "ja";
	WordSpace space;
	space.count = 1;
	WordNormal b;
	b.content  = "b";
	b.language = "en";
	WordNormal c;
	c.content                     = "c";
	c.language                    = "ja";
	const std::vector<Word> words = {
		makeWordNormal(a),
		makeWordSpace(space),
		makeWordNormal(b),
		makeWordNormal(c),
	};

	const std::vector<std::string> tags = getWordsLanguages(words);
	REQUIRE(tags.size() == 2);
	CHECK(tags[0] == "ja");
	CHECK(tags[1] == "en");
}

TEST_CASE("getActiveWord picks the timed word covering the moment") {
	WordNormal first;
	first.content = "a";
	first.time    = Time{100, 200};
	WordSpace space;
	space.count = 1;
	WordNormal second;
	second.content                = "b";
	second.time                   = Time{200, 300};
	const std::vector<Word> words = {
		makeWordNormal(first),
		makeWordSpace(space),
		makeWordNormal(second),
	};

	CHECK(getActiveWordIndex(words, 150) == 0);
	CHECK(asWordNormal(*getActiveWord(words, 250))->content == "b");
	CHECK(getActiveWordIndex(words, 50) == -1);
	CHECK(getActiveWord(words, 50) == nullptr);
}

TEST_CASE("getWordRubies and getWordAnnotationText read annotation tokens") {
	WordAnnotationRuby    ruby;
	WordAnnotationContent token;
	token.content = "kyou";
	ruby.words.push_back(token);

	WordAnnotation annotation;
	annotation.rubies.push_back(ruby);

	WordNormal normal;
	normal.content    = "today";
	normal.annotation = annotation;
	const Word word   = makeWordNormal(normal);

	const auto rubies = getWordRubies(word);
	REQUIRE(rubies.size() == 1);
	CHECK(getWordAnnotationText(*rubies[0]) == "kyou");
	CHECK(getWordRubies(makeWordSpace(WordSpace{})).empty());
}
