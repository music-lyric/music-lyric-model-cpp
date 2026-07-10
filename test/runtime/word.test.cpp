#include "doctest.h"
#include "music_lyric_model.h"

using namespace music_lyric_model::runtime;
using namespace lyric::common;
using namespace lyric::runtime;

TEST_CASE("word annotation makers preserve initial values") {
	WordAnnotationContent content;
	content.set_content("ni");
	CHECK(makeWordAnnotationContent(content).content() == "ni");
	WordAnnotationRoman roman;
	*roman.add_words() = content;
	CHECK(makeWordAnnotationRoman(roman).words(0).content() == "ni");
	WordAnnotationTranslate translate;
	translate.set_content("hello");
	CHECK(makeWordAnnotationTranslate(translate).content() == "hello");
	CHECK(makeWordAnnotationRuby().words().empty());
	CHECK(makeWordAnnotationUnknown().key().empty());
	CHECK(makeWordAnnotation().unknowns().empty());
}

TEST_CASE("guards narrow the word body") {
	WordNormal n;
	n.set_content("hi");
	const Word normal = makeWordNormal(n);
	WordSpace  s;
	s.set_count(2);
	const Word space = makeWordSpace(s);
	CHECK(isWordNormal(normal) == true);
	CHECK(isWordSpace(space) == true);
	CHECK(isWordNormal(space) == false);
}

TEST_CASE("asWordNormal and asWordSpace unwrap the matching variant") {
	WordNormal n;
	n.set_content("hi");
	const Word normal = makeWordNormal(n);
	WordSpace  s;
	s.set_count(2);
	const Word space = makeWordSpace(s);
	REQUIRE(asWordNormal(normal) != nullptr);
	CHECK(asWordNormal(normal)->content() == "hi");
	CHECK(asWordNormal(space) == nullptr);
	REQUIRE(asWordSpace(space) != nullptr);
	CHECK(asWordSpace(space)->count() == 2);
	CHECK(asWordSpace(normal) == nullptr);
}

TEST_CASE("getWordText reads content or padded spaces") {
	WordNormal n;
	n.set_content("hi");
	WordSpace s;
	s.set_count(3);
	CHECK(getWordText(makeWordNormal(n)) == "hi");
	CHECK(getWordText(makeWordSpace(s)) == "   ");
}

TEST_CASE("getWordTime and getWordDuration accept a Word or a bare WordNormal") {
	WordNormal n;
	n.set_content("hi");
	n.mutable_time()->set_start(1000);
	n.mutable_time()->set_end(1500);
	const Word        word   = makeWordNormal(n);
	const WordNormal* normal = asWordNormal(word);
	REQUIRE(normal != nullptr);
	REQUIRE(getWordTime(word) != nullptr);
	CHECK(getWordTime(word)->start() == 1000);
	REQUIRE(getWordTime(*normal) != nullptr);
	CHECK(getWordTime(*normal)->start() == 1000);
	CHECK(getWordDuration(word) == 500);
	CHECK(getWordDuration(*normal) == 500);
}

TEST_CASE("getWordTime is absent and getWordDuration zero for a space") {
	WordSpace s;
	s.set_count(1);
	const Word space = makeWordSpace(s);
	CHECK(getWordTime(space) == nullptr);
	CHECK(getWordDuration(space) == 0);
}

TEST_CASE("getWordRuby reads the ruby annotation") {
	WordAnnotationRuby ruby;
	ruby.add_words()->set_content("かん");
	WordNormal n;
	n.set_content("漢");
	*n.mutable_annotation()->mutable_ruby() = ruby;
	const Word word                         = makeWordNormal(n);
	REQUIRE(getWordRuby(word) != nullptr);
	CHECK(getWordRuby(word)->words(0).content() == "かん");
	WordNormal plain;
	plain.set_content("a");
	CHECK(getWordRuby(makeWordNormal(plain)) == nullptr);
}

TEST_CASE("getWordAnnotationText joins tokens in order") {
	WordAnnotationRuby ruby;
	ruby.add_words()->set_content("か");
	ruby.add_words()->set_content("ん");
	CHECK(getWordAnnotationText(ruby) == "かん");
	WordAnnotationRoman roman;
	roman.add_words()->set_content("ni");
	roman.add_words()->set_content("hao");
	CHECK(getWordAnnotationText(roman) == "nihao");
}
