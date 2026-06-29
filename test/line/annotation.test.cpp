#include "line/annotation.h"

#include <string>

#include "doctest.h"
#include "line/content.h"

using namespace music_lyric_model;
using namespace lyric;
using google::protobuf::RepeatedPtrField;

namespace {
	RepeatedPtrField<LineAnnotationTranslate> makeTranslates() {
		RepeatedPtrField<LineAnnotationTranslate> translates;
		LineAnnotationTranslate*                  zh = translates.Add();
		zh->set_content("你好");
		zh->set_language("zh-hans");
		LineAnnotationTranslate* en = translates.Add();
		en->set_content("hi");
		en->set_language("en");
		return translates;
	}

	Line makeAnnotatedLine() {
		LineAnnotation annotation;
		*annotation.mutable_translates() = makeTranslates();
		annotation.add_romans()->set_content("nihao");
		LineNormal body;
		*body.mutable_annotation() = annotation;
		return makeLineNormal(body);
	}
} // namespace

TEST_CASE("getFirstAnnotation prefers a language match then falls back") {
	const RepeatedPtrField<LineAnnotationTranslate> translates = makeTranslates();
	REQUIRE(getFirstAnnotation(translates, std::string("en")) != nullptr);
	CHECK(getFirstAnnotation(translates, std::string("en"))->content() == "hi");
	REQUIRE(getFirstAnnotation(translates) != nullptr);
	CHECK(getFirstAnnotation(translates)->content() == "你好");
	REQUIRE(getFirstAnnotation(translates, std::string("ko")) != nullptr);
	CHECK(getFirstAnnotation(translates, std::string("ko"))->content() == "你好");
	CHECK(getFirstAnnotation(RepeatedPtrField<LineAnnotationTranslate>()) == nullptr);
}

TEST_CASE("getLineTranslate reads the translation") {
	const Line line = makeAnnotatedLine();
	CHECK(getLineTranslate(line) == std::string("你好"));
	CHECK(getLineTranslate(line, std::string("en")) == std::string("hi"));
}

TEST_CASE("getLineRoman reads the romanization") {
	const Line line = makeAnnotatedLine();
	CHECK(getLineRoman(line) == std::string("nihao"));
}
