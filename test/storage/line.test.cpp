#include "storage/line.h"

#include <string>
#include <vector>

#include "doctest.h"
#include "storage/word.h"

namespace Storage = music_lyric_model::storage;

using namespace lyric::storage;
using google::protobuf::RepeatedPtrField;

namespace {
	/**
	 * Builds a timed storage line with words and annotations.
	 */
	Line makeSampleLine() {
		WordNormal first;
		first.set_content("hello");
		first.set_language("en");
		WordSpace space;
		space.set_count(1);
		WordNormal second;
		second.set_content("世界");
		second.set_language("zh");

		Line         line;
		LineContent* content = line.mutable_content();
		content->mutable_time()->set_start(1000);
		content->mutable_time()->set_end(2000);
		*content->add_words() = Storage::makeWordNormal(first);
		*content->add_words() = Storage::makeWordSpace(space);
		*content->add_words() = Storage::makeWordNormal(second);
		content->mutable_annotation()->add_translates()->set_content("hello world");
		content->mutable_annotation()->add_romans()->set_content("hello shijie");
		return Storage::makeLine(line);
	}
} // namespace

TEST_CASE("storage line makers preserve initial values") {
	LineContent content;
	content.mutable_time()->set_start(1000);
	CHECK(Storage::makeLineContent(content).time().start() == 1000);
	Line line;
	*line.mutable_content() = content;
	CHECK(Storage::makeLine(line).content().time().start() == 1000);
	LineAnnotationUnknown unknown;
	unknown.set_key("mood");
	CHECK(Storage::makeLineAnnotationUnknown(unknown).key() == "mood");
	LineAnnotationTranslate translate;
	translate.set_content("hello");
	CHECK(Storage::makeLineAnnotationTranslate(translate).content() == "hello");
	LineAnnotationRoman roman;
	roman.set_content("nihao");
	CHECK(Storage::makeLineAnnotationRoman(roman).content() == "nihao");
	LineAnnotation annotation;
	*annotation.add_translates() = translate;
	CHECK(Storage::makeLineAnnotation(annotation).translates(0).content() == "hello");
}

TEST_CASE("storage line content time duration and text are readable") {
	const Line line = makeSampleLine();
	REQUIRE(Storage::getLineContent(line) != nullptr);
	REQUIRE(Storage::getLineTime(line) != nullptr);
	CHECK(Storage::getLineTime(line)->start() == 1000);
	CHECK(Storage::getLineDuration(line) == 1000);
	CHECK(Storage::getLineWords(line).size() == 3);
	CHECK(Storage::getWordsText(Storage::getLineWords(line)) == "hello 世界");
	CHECK(Storage::getLineText(line) == "hello 世界");
	CHECK(Storage::getLineContent(Line()) == nullptr);
	CHECK(Storage::getLineWords(Line()).empty());
}

TEST_CASE("storage line languages are collected from words") {
	const Line line = makeSampleLine();
	CHECK(Storage::getWordsLanguages(Storage::getLineWords(line)) == std::vector<std::string>{"en", "zh"});
	CHECK(Storage::getLineLanguages(line) == std::vector<std::string>{"en", "zh"});
}

TEST_CASE("storage line annotations prefer a language match then fall back") {
	Line            line       = makeSampleLine();
	LineAnnotation* annotation = line.mutable_content()->mutable_annotation();
	annotation->mutable_translates(0)->set_language("en");
	LineAnnotationTranslate* chinese = annotation->add_translates();
	chinese->set_language("zh");
	chinese->set_content("你好世界");
	REQUIRE(Storage::getLineAnnotation(line) != nullptr);
	CHECK(Storage::getLineTranslate(line, std::string("zh")) == std::string("你好世界"));
	CHECK(Storage::getLineTranslate(line, std::string("ko")) == std::string("hello world"));
	CHECK(Storage::getLineRoman(line) == std::string("hello shijie"));
}

TEST_CASE("storage active line lookup uses content time") {
	Line first = makeSampleLine();
	Line second;
	second.mutable_content()->mutable_time()->set_start(2000);
	second.mutable_content()->mutable_time()->set_end(3000);
	RepeatedPtrField<Line> lines;
	*lines.Add() = first;
	*lines.Add() = second;
	CHECK(Storage::getActiveLineIndex(lines, 1500) == 0);
	CHECK(Storage::getActiveLineIndex(lines, 2500) == 1);
	CHECK(Storage::getActiveLineIndex(lines, 3000) == -1);
	REQUIRE(Storage::getActiveLine(lines, 2500) != nullptr);
	CHECK(Storage::getActiveLine(lines, 2500) == &lines.Get(1));
}
