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

	/**
	 * Build a normal word carrying an unknown annotation.
	 */
	lyric::common::Word unknownWord(const std::string& content, const std::string& key, const std::string& value) {
		lyric::common::Unknown unknown;
		unknown.set_key(key);
		unknown.set_value(value);

		lyric::common::WordAnnotation annotation;
		*annotation.add_unknowns() = makeUnknown(unknown);

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

TEST_CASE("deriveParsedLineUnknowns and deriveParsedLineAnnotation group by key") {
	google::protobuf::RepeatedPtrField<lyric::common::Word> words;
	*words.Add() = unknownWord("a", "ruby", "x");
	lyric::common::WordSpace space;
	space.set_count(1);
	*words.Add() = makeWordSpace(space);
	*words.Add() = unknownWord("b", "ruby", "y");

	const auto unknowns = deriveParsedLineUnknowns(words);
	REQUIRE(unknowns.size() == 1);
	CHECK(unknowns[0].key() == "ruby");
	CHECK(unknowns[0].value() == "x y");

	const lyric::common::LineAnnotation annotation = deriveParsedLineAnnotation(words);
	REQUIRE(annotation.unknowns_size() == 1);
	CHECK(annotation.unknowns(0).value() == "x y");
}
