#include "word/content.h"

#include "doctest.h"
#include "word/annotation.h"

using namespace music_lyric_model;
using namespace lyric;

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

TEST_CASE("getWordText reads content or padded spaces") {
	WordNormal n;
	n.set_content("hi");
	WordSpace s;
	s.set_count(3);
	CHECK(getWordText(makeWordNormal(n)) == "hi");
	CHECK(getWordText(makeWordSpace(s)) == "   ");
}

TEST_CASE("getWordTime is present only on a normal word") {
	WordNormal n;
	n.mutable_time()->set_start(10);
	n.mutable_time()->set_end(20);
	const Word normal = makeWordNormal(n);
	REQUIRE(getWordTime(normal) != nullptr);
	CHECK(getWordTime(normal)->end() == 20);
	WordSpace s;
	s.set_count(1);
	CHECK(getWordTime(makeWordSpace(s)) == nullptr);
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
