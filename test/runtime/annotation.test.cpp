#include <string>

#include "doctest.h"
#include "runtime/line.h"
#include "runtime/word.h"

using namespace music_lyric_model;
using namespace lyric::common;
using namespace lyric::runtime;
using google::protobuf::RepeatedPtrField;

namespace {
	/**
	 * Builds sample line translations.
	 */
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

	/**
	 * Builds a normal line with line-level annotations.
	 */
	Line makeAnnotatedLine() {
		LineAnnotation annotation;
		*annotation.mutable_translates() = makeTranslates();
		annotation.add_romans()->set_content("nihao");
		LineNormal body;
		*body.mutable_content()->mutable_annotation() = annotation;
		return makeLineNormal(body);
	}

	/**
	 * Builds a normal word with one romanization item.
	 */
	Word makeRomanWord(const std::string& content, const std::string& roman, const std::string& language = "") {
		WordNormal word;
		word.set_content(content);
		WordAnnotationRoman* item = word.mutable_annotation()->add_romans();
		item->add_words()->set_content(roman);
		if (!language.empty()) {
			item->set_language(language);
		}
		return makeWordNormal(word);
	}

	/**
	 * Builds a normal word with one translation item.
	 */
	Word makeTranslateWord(const std::string& content, const std::string& translate, const std::string& language) {
		WordNormal word;
		word.set_content(content);
		WordAnnotationTranslate* item = word.mutable_annotation()->add_translates();
		item->set_language(language);
		item->set_content(translate);
		return makeWordNormal(word);
	}

	/**
	 * Builds a normal word with one unknown annotation item.
	 */
	Word makeUnknownWord(const std::string& content, const std::string& key, const std::string& value) {
		WordNormal word;
		word.set_content(content);
		WordAnnotationUnknown* item = word.mutable_annotation()->add_unknowns();
		item->set_key(key);
		item->set_value(value);
		return makeWordNormal(word);
	}

	/**
	 * Places one space token between two words.
	 */
	RepeatedPtrField<Word> makeSpacedWords(const Word& first, const Word& second) {
		WordSpace space;
		space.set_count(1);
		RepeatedPtrField<Word> words;
		*words.Add() = first;
		*words.Add() = makeWordSpace(space);
		*words.Add() = second;
		return words;
	}
} // namespace

TEST_CASE("line annotation makers preserve initial values") {
	LineAnnotationTranslate translate;
	translate.set_content("hello");
	CHECK(makeLineAnnotationTranslate(translate).content() == "hello");
	LineAnnotationRoman roman;
	roman.set_content("nihao");
	CHECK(makeLineAnnotationRoman(roman).content() == "nihao");
	LineAnnotationUnknown unknown;
	unknown.set_key("mood");
	CHECK(makeLineAnnotationUnknown(unknown).key() == "mood");
	LineAnnotation annotation;
	*annotation.add_translates() = translate;
	CHECK(makeLineAnnotation(annotation).translates(0).content() == "hello");
}

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

TEST_CASE("deriveLineRomans joins tokens in word order with padded spacing") {
	const RepeatedPtrField<Word>           words  = makeSpacedWords(makeRomanWord("今日", "kyou", "romaji"), makeRomanWord("は", "wa", "romaji"));
	const std::vector<LineAnnotationRoman> romans = deriveLineRomans(words);
	REQUIRE(romans.size() == 1);
	CHECK(romans.at(0).content() == "kyou wa");
	CHECK(romans.at(0).language() == "romaji");
	CHECK(romans.at(0).derived() == true);
}

TEST_CASE("deriveLineTranslates aggregates word translations by language") {
	const RepeatedPtrField<Word>               words      = makeSpacedWords(makeTranslateWord("猫", "cat", "en"), makeTranslateWord("好", "good", "en"));
	const std::vector<LineAnnotationTranslate> translates = deriveLineTranslates(words);
	REQUIRE(translates.size() == 1);
	CHECK(translates.at(0).content() == "cat good");
	CHECK(translates.at(0).language() == "en");
	CHECK(translates.at(0).derived() == true);
}

TEST_CASE("deriveLineUnknowns aggregates word unknowns by original key") {
	const RepeatedPtrField<Word>             words    = makeSpacedWords(makeUnknownWord("a", "mood", "calm"), makeUnknownWord("b", "mood", "bright"));
	const std::vector<LineAnnotationUnknown> unknowns = deriveLineUnknowns(words);
	REQUIRE(unknowns.size() == 1);
	CHECK(unknowns.at(0).key() == "mood");
	CHECK(unknowns.at(0).value() == "calm bright");
	CHECK(unknowns.at(0).derived() == true);
}

TEST_CASE("deriveLineAnnotation marks every item derived") {
	const RepeatedPtrField<Word> words      = makeSpacedWords(makeRomanWord("你", "ni"), makeRomanWord("好", "hao"));
	const LineAnnotation         annotation = deriveLineAnnotation(words);
	REQUIRE(annotation.romans_size() == 1);
	CHECK(annotation.romans(0).content() == "ni hao");
	CHECK(annotation.romans(0).derived() == true);
}
