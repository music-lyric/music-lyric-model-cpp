#include <optional>
#include <string>
#include <vector>

#include "doctest.h"
#include "music_lyric_model.h"

using namespace music_lyric_model::common;
using namespace music_lyric_model::parsed;

TEST_CASE("asParsedLineNormal and asParsedLineInterlude unwrap the matching variant") {
	LineNormal normalInit;
	WordNormal wordInit;
	wordInit.content     = "hi";
	normalInit.words     = {makeWordNormal(wordInit)};
	const Line normal    = makeParsedLineNormal(normalInit);
	const Line interlude = makeParsedLineInterlude();

	CHECK(asParsedLineNormal(normal)->words.size() == 1);
	CHECK(asParsedLineNormal(interlude) == nullptr);
	CHECK(asParsedLineInterlude(interlude) != nullptr);
	CHECK(asParsedLineInterlude(normal) == nullptr);
}

TEST_CASE("getParsedLineTime duration words and languages cover wrapper and bodies") {
	LineNormal normalInit;
	normalInit.time      = Time{1000, 1500};
	normalInit.languages = {"ja"};
	WordNormal wordInit;
	wordInit.content          = "hi";
	wordInit.language         = "en";
	normalInit.words          = {makeWordNormal(wordInit)};
	const Line        wrapper = makeParsedLineNormal(normalInit);
	const LineNormal* normal  = asParsedLineNormal(wrapper);

	CHECK(getParsedLineTime(wrapper)->start == 1000);
	CHECK(getParsedLineTime(*normal)->start == 1000);
	CHECK(getParsedLineDuration(wrapper) == 500);
	CHECK(getParsedLineText(wrapper) == "hi");
	CHECK(getParsedLineLanguages(wrapper) == std::vector<std::string>{"ja"});

	LineBackground background;
	background.time  = Time{2000, 2300};
	background.words = {makeWordNormal(wordInit)};
	CHECK(getParsedLineTime(background)->start == 2000);
	CHECK(getParsedLineDuration(background) == 300);
	CHECK(getParsedLineLanguages(background) == std::vector<std::string>{"en"});

	LineInterlude interludeInit;
	interludeInit.time   = Time{3000, 3500};
	const Line interlude = makeParsedLineInterlude(interludeInit);
	CHECK(getParsedLineDuration(interlude) == 500);
	CHECK(getParsedLineWords(interlude).empty());
	CHECK(getParsedLineLanguages(interlude).empty());
	CHECK(getParsedLineAnnotation(interlude) == nullptr);
}

TEST_CASE("getParsedActiveLine picks the timed line covering the moment") {
	LineNormal first;
	first.time = Time{1000, 2000};
	LineNormal second;
	second.time = Time{2000, 3000};
	std::vector<Line> lines;
	lines.push_back(makeParsedLineNormal(first));
	lines.push_back(makeParsedLineNormal(second));

	CHECK(getParsedActiveLineIndex(lines, 1500) == 0);
	CHECK(getParsedActiveLine(lines, 2500) == &lines[1]);
	CHECK(getParsedActiveLineIndex(lines, 50) == -1);
	CHECK(getParsedActiveLine(lines, 50) == nullptr);
}

TEST_CASE("getParsedLineTranslation and getParsedLineRoman read line annotations") {
	LineAnnotation            annotation;
	LineAnnotationTranslation translation;
	translation.language = "en";
	translation.content  = "hello";
	annotation.translations.push_back(translation);
	LineAnnotationRoman roman;
	roman.language = "romaji";
	roman.content  = "konnichiwa";
	annotation.romans.push_back(roman);

	LineNormal normalInit;
	normalInit.annotation = annotation;
	const Line line       = makeParsedLineNormal(normalInit);

	CHECK(*getParsedLineTranslation(line, std::string("en")) == "hello");
	CHECK(*getParsedLineRoman(line, std::string("romaji")) == "konnichiwa");
	CHECK(*getParsedLineTranslation(*asParsedLineNormal(line)) == "hello");
}
