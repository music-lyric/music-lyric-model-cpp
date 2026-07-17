#include <optional>
#include <string>

#include "doctest.h"
#include "music_lyric_model.h"

using namespace music_lyric_model::common;
using namespace music_lyric_model::parsed;

namespace {
	/**
	 * Build a normal word carrying a single-token roman annotation.
	 */
	lyric::common::Word romanWord(const std::string& content, const std::string& roman, const std::optional<std::string>& language = std::nullopt) {
		lyric::common::WordAnnotationContent token;
		token.set_content(roman);

		lyric::common::WordAnnotationRoman romanItem;
		if (language.has_value()) {
			romanItem.set_language(*language);
		}
		*romanItem.add_words() = makeWordAnnotationContent(token);

		lyric::common::WordAnnotation annotation;
		*annotation.add_romans() = makeWordAnnotationRoman(romanItem);

		lyric::common::WordNormal normal;
		normal.set_content(content);
		*normal.mutable_annotation() = makeWordAnnotation(annotation);
		return makeWordNormal(normal);
	}

	/**
	 * Build a normal word carrying a single translation.
	 */
	lyric::common::Word translateWord(const std::string& content, const std::string& translation, const std::optional<std::string>& language = std::nullopt) {
		lyric::common::WordAnnotationTranslation item;
		if (language.has_value()) {
			item.set_language(*language);
		}
		item.set_content(translation);

		lyric::common::WordAnnotation annotation;
		*annotation.add_translations() = makeWordAnnotationTranslation(item);

		lyric::common::WordNormal normal;
		normal.set_content(content);
		*normal.mutable_annotation() = makeWordAnnotation(annotation);
		return makeWordNormal(normal);
	}
} // namespace

TEST_CASE("deriveParsedLineRomans joins tokens in word order with padded spacing") {
	google::protobuf::RepeatedPtrField<lyric::common::Word> words;
	*words.Add() = romanWord("今日", "kyou", "romaji");
	lyric::common::WordSpace space;
	space.set_count(1);
	*words.Add() = makeWordSpace(space);
	*words.Add() = romanWord("は", "wa", "romaji");

	const auto items = deriveParsedLineRomans(words);
	REQUIRE(items.size() == 1);
	CHECK(items[0].content() == "kyou wa");
	CHECK(items[0].language() == "romaji");
}

TEST_CASE("deriveParsedLineTranslations aggregates word translations by language") {
	google::protobuf::RepeatedPtrField<lyric::common::Word> words;
	*words.Add() = translateWord("猫", "cat", "en");
	lyric::common::WordSpace space;
	space.set_count(1);
	*words.Add() = makeWordSpace(space);
	*words.Add() = translateWord("好", "good", "en");

	const auto items = deriveParsedLineTranslations(words);
	REQUIRE(items.size() == 1);
	CHECK(items[0].content() == "cat good");
	CHECK(items[0].language() == "en");
}

TEST_CASE("getFirstAnnotation prefers a language match then falls back") {
	google::protobuf::RepeatedPtrField<lyric::common::LineAnnotationTranslation> items;
	lyric::common::LineAnnotationTranslation                                     zh;
	zh.set_content("你好");
	zh.set_language("zh");
	*items.Add() = makeLineAnnotationTranslation(zh);
	lyric::common::LineAnnotationTranslation en;
	en.set_content("hi");
	en.set_language("en");
	*items.Add() = makeLineAnnotationTranslation(en);

	CHECK(getFirstAnnotation(items, std::string("en"))->content() == "hi");
	CHECK(getFirstAnnotation(items)->content() == "你好");
	CHECK(getFirstAnnotation(items, std::string("ko"))->content() == "你好");
}
