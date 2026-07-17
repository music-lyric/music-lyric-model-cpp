#include <optional>
#include <string>
#include <vector>

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

TEST_CASE("getParsedLineTime duration words and languages cover wrapper and bodies") {
	lyric::parsed::LineNormal normalInit;
	normalInit.mutable_time()->set_start(1000);
	normalInit.mutable_time()->set_end(1500);
	normalInit.add_languages("ja");
	lyric::common::WordNormal wordInit;
	wordInit.set_content("hi");
	wordInit.set_language("en");
	*normalInit.add_words() = makeWordNormal(wordInit);
	const lyric::parsed::Line wrapper = makeParsedLineNormal(normalInit);
	const lyric::parsed::LineNormal* normal = asParsedLineNormal(wrapper);

	CHECK(getParsedLineTime(wrapper)->start() == 1000);
	CHECK(getParsedLineTime(*normal)->start() == 1000);
	CHECK(getParsedLineDuration(wrapper) == 500);
	CHECK(getParsedLineText(wrapper) == "hi");
	CHECK(getParsedLineLanguages(wrapper) == std::vector<std::string>{"ja"});

	lyric::parsed::LineBackground background;
	background.mutable_time()->set_start(2000);
	background.mutable_time()->set_end(2300);
	*background.add_words() = makeWordNormal(wordInit);
	CHECK(getParsedLineTime(background)->start() == 2000);
	CHECK(getParsedLineDuration(background) == 300);
	CHECK(getParsedLineLanguages(background) == std::vector<std::string>{"en"});

	lyric::parsed::LineInterlude interludeInit;
	interludeInit.mutable_time()->set_start(3000);
	interludeInit.mutable_time()->set_end(3500);
	const lyric::parsed::Line interlude = makeParsedLineInterlude(interludeInit);
	CHECK(getParsedLineDuration(interlude) == 500);
	CHECK(getParsedLineWords(interlude).empty());
	CHECK(getParsedLineLanguages(interlude).empty());
	CHECK(getParsedLineAnnotation(interlude) == nullptr);
}

TEST_CASE("getParsedActiveLine picks the timed line covering the moment") {
	google::protobuf::RepeatedPtrField<lyric::parsed::Line> lines;

	lyric::parsed::LineNormal first;
	first.mutable_time()->set_start(1000);
	first.mutable_time()->set_end(2000);
	*lines.Add() = makeParsedLineNormal(first);

	lyric::parsed::LineNormal second;
	second.mutable_time()->set_start(2000);
	second.mutable_time()->set_end(3000);
	*lines.Add() = makeParsedLineNormal(second);

	CHECK(getParsedActiveLineIndex(lines, 1500) == 0);
	CHECK(getParsedActiveLine(lines, 2500) == &lines.Get(1));
	CHECK(getParsedActiveLineIndex(lines, 50) == -1);
	CHECK(getParsedActiveLine(lines, 50) == nullptr);
}

TEST_CASE("getParsedLineTranslation and getParsedLineRoman read line annotations") {
	lyric::common::LineAnnotationTranslation translation;
	translation.set_language("en");
	translation.set_content("hello");
	lyric::common::LineAnnotationRoman roman;
	roman.set_language("romaji");
	roman.set_content("konnichiwa");
	lyric::common::LineAnnotation annotation;
	*annotation.add_translations() = makeLineAnnotationTranslation(translation);
	*annotation.add_romans()       = makeLineAnnotationRoman(roman);

	lyric::parsed::LineNormal normalInit;
	*normalInit.mutable_annotation() = makeLineAnnotation(annotation);
	const lyric::parsed::Line line = makeParsedLineNormal(normalInit);

	CHECK(*getParsedLineTranslation(line, std::string("en")) == "hello");
	CHECK(*getParsedLineRoman(line, std::string("romaji")) == "konnichiwa");
	CHECK(*getParsedLineTranslation(*asParsedLineNormal(line)) == "hello");
}
