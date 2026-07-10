#include <string>
#include <vector>

#include "doctest.h"
#include "music_lyric_model.h"

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
		*content->add_words() = music_lyric_model::storage::makeWordNormal(first);
		*content->add_words() = music_lyric_model::storage::makeWordSpace(space);
		*content->add_words() = music_lyric_model::storage::makeWordNormal(second);
		content->mutable_annotation()->add_translates()->set_content("hello world");
		content->mutable_annotation()->add_romans()->set_content("hello shijie");
		return music_lyric_model::storage::makeLine(line);
	}
} // namespace

TEST_CASE("storage line makers preserve initial values") {
	LineContent content;
	content.mutable_time()->set_start(1000);
	CHECK(music_lyric_model::storage::makeLineContent(content).time().start() == 1000);
	Line line;
	*line.mutable_content() = content;
	CHECK(music_lyric_model::storage::makeLine(line).content().time().start() == 1000);
	LineAnnotationUnknown unknown;
	unknown.set_key("mood");
	CHECK(music_lyric_model::storage::makeLineAnnotationUnknown(unknown).key() == "mood");
	LineAnnotationTranslate translate;
	translate.set_content("hello");
	CHECK(music_lyric_model::storage::makeLineAnnotationTranslate(translate).content() == "hello");
	LineAnnotationRoman roman;
	roman.set_content("nihao");
	CHECK(music_lyric_model::storage::makeLineAnnotationRoman(roman).content() == "nihao");
	LineAnnotation annotation;
	*annotation.add_translates() = translate;
	CHECK(music_lyric_model::storage::makeLineAnnotation(annotation).translates(0).content() == "hello");
}

TEST_CASE("storage line content time duration and text are readable") {
	const Line line = makeSampleLine();
	REQUIRE(music_lyric_model::storage::getLineContent(line) != nullptr);
	REQUIRE(music_lyric_model::storage::getLineTime(line) != nullptr);
	CHECK(music_lyric_model::storage::getLineTime(line)->start() == 1000);
	CHECK(music_lyric_model::storage::getLineDuration(line) == 1000);
	CHECK(music_lyric_model::storage::getLineWords(line).size() == 3);
	CHECK(music_lyric_model::storage::getWordsText(music_lyric_model::storage::getLineWords(line)) == "hello 世界");
	CHECK(music_lyric_model::storage::getLineText(line) == "hello 世界");
	CHECK(music_lyric_model::storage::getLineContent(Line()) == nullptr);
	CHECK(music_lyric_model::storage::getLineWords(Line()).empty());
}

TEST_CASE("storage line languages are collected from words") {
	const Line line = makeSampleLine();
	CHECK(music_lyric_model::storage::getWordsLanguages(music_lyric_model::storage::getLineWords(line)) == std::vector<std::string>{"en", "zh"});
	CHECK(music_lyric_model::storage::getLineLanguages(line) == std::vector<std::string>{"en", "zh"});
}

TEST_CASE("storage line annotations prefer a language match then fall back") {
	Line            line       = makeSampleLine();
	LineAnnotation* annotation = line.mutable_content()->mutable_annotation();
	annotation->mutable_translates(0)->set_language("en");
	LineAnnotationTranslate* chinese = annotation->add_translates();
	chinese->set_language("zh");
	chinese->set_content("你好世界");
	REQUIRE(music_lyric_model::storage::getLineAnnotation(line) != nullptr);
	CHECK(music_lyric_model::storage::getLineTranslate(line, std::string("zh")) == std::string("你好世界"));
	CHECK(music_lyric_model::storage::getLineTranslate(line, std::string("ko")) == std::string("hello world"));
	CHECK(music_lyric_model::storage::getLineRoman(line) == std::string("hello shijie"));
}

TEST_CASE("storage active line lookup uses content time") {
	Line first = makeSampleLine();
	Line second;
	second.mutable_content()->mutable_time()->set_start(2000);
	second.mutable_content()->mutable_time()->set_end(3000);
	RepeatedPtrField<Line> lines;
	*lines.Add() = first;
	*lines.Add() = second;
	CHECK(music_lyric_model::storage::getActiveLineIndex(lines, 1500) == 0);
	CHECK(music_lyric_model::storage::getActiveLineIndex(lines, 2500) == 1);
	CHECK(music_lyric_model::storage::getActiveLineIndex(lines, 3000) == -1);
	REQUIRE(music_lyric_model::storage::getActiveLine(lines, 2500) != nullptr);
	CHECK(music_lyric_model::storage::getActiveLine(lines, 2500) == &lines.Get(1));
}
