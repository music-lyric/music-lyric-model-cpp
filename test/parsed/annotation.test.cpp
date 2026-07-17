#include <optional>
#include <string>
#include <vector>

#include "doctest.h"
#include "music_lyric_model.h"

using namespace music_lyric_model::common;
using namespace music_lyric_model::parsed;

namespace {
	/**
	 * Build a normal word carrying a single-token roman annotation.
	 */
	Word romanWord(const std::string& content, const std::string& roman, const std::optional<std::string>& language = std::nullopt) {
		WordAnnotationContent token;
		token.content = roman;

		WordAnnotationRoman romanItem;
		romanItem.language = language;
		romanItem.words.push_back(token);

		WordAnnotation annotation;
		annotation.romans.push_back(romanItem);

		WordNormal normal;
		normal.content    = content;
		normal.annotation = annotation;
		return makeWordNormal(normal);
	}

	/**
	 * Build a normal word carrying a single translation.
	 */
	Word translateWord(const std::string& content, const std::string& translation, const std::optional<std::string>& language = std::nullopt) {
		WordAnnotationTranslation item;
		item.language = language;
		item.content  = translation;

		WordAnnotation annotation;
		annotation.translations.push_back(item);

		WordNormal normal;
		normal.content    = content;
		normal.annotation = annotation;
		return makeWordNormal(normal);
	}

	/**
	 * Build a normal word carrying an unknown annotation.
	 */
	Word unknownWord(const std::string& content, const std::string& key, const std::string& value) {
		WordAnnotation annotation;
		annotation.unknowns.push_back(Unknown{key, value});

		WordNormal normal;
		normal.content    = content;
		normal.annotation = annotation;
		return makeWordNormal(normal);
	}
} // namespace

TEST_CASE("deriveParsedLineRomans joins tokens in word order with padded spacing") {
	WordSpace space;
	space.count                   = 1;
	const std::vector<Word> words = {
		romanWord("A", "kyou", "romaji"),
		makeWordSpace(space),
		romanWord("B", "wa", "romaji"),
	};

	const auto items = deriveParsedLineRomans(words);
	REQUIRE(items.size() == 1);
	CHECK(items[0].content == "kyou wa");
	CHECK(items[0].language == "romaji");
}

TEST_CASE("deriveParsedLineTranslations aggregates word translations by language") {
	WordSpace space;
	space.count                   = 1;
	const std::vector<Word> words = {
		translateWord("A", "cat", "en"),
		makeWordSpace(space),
		translateWord("B", "good", "en"),
	};

	const auto items = deriveParsedLineTranslations(words);
	REQUIRE(items.size() == 1);
	CHECK(items[0].content == "cat good");
	CHECK(items[0].language == "en");
}

TEST_CASE("deriveParsedLineUnknowns and deriveParsedLineAnnotation group by key") {
	WordSpace space;
	space.count                   = 1;
	const std::vector<Word> words = {
		unknownWord("a", "ruby", "x"),
		makeWordSpace(space),
		unknownWord("b", "ruby", "y"),
	};

	const auto unknowns = deriveParsedLineUnknowns(words);
	REQUIRE(unknowns.size() == 1);
	CHECK(unknowns[0].key == "ruby");
	CHECK(unknowns[0].value == "x y");

	const LineAnnotation annotation = deriveParsedLineAnnotation(words);
	REQUIRE(annotation.unknowns.size() == 1);
	CHECK(annotation.unknowns[0].value == "x y");
}
