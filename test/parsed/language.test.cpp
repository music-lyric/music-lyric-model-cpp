#include "doctest.h"
#include "music_lyric_model.h"

using namespace music_lyric_model::parsed;

namespace {
	/**
	 * Build a sample language distribution.
	 */
	google::protobuf::RepeatedPtrField<lyric::parsed::LanguageItem> makeLanguages() {
		google::protobuf::RepeatedPtrField<lyric::parsed::LanguageItem> languages;
		lyric::parsed::LanguageItem                                     en;
		en.set_tag("en");
		en.set_percent(70);
		*languages.Add() = makeParsedLanguageItem(en);
		lyric::parsed::LanguageItem ja;
		ja.set_tag("ja");
		ja.set_percent(30);
		*languages.Add() = makeParsedLanguageItem(ja);
		return languages;
	}
} // namespace

TEST_CASE("makeParsedLanguageItem preserves initial values") {
	lyric::parsed::LanguageItem init;
	init.set_tag("en");
	init.set_percent(70);
	const lyric::parsed::LanguageItem item = makeParsedLanguageItem(init);
	CHECK(item.tag() == "en");
	CHECK(item.percent() == 70);
}

TEST_CASE("hasParsedLanguage checks presence by tag") {
	const auto languages = makeLanguages();
	CHECK(hasParsedLanguage(languages, "ja") == true);
	CHECK(hasParsedLanguage(languages, "ko") == false);
}

TEST_CASE("getParsedLanguageByTag finds the entry") {
	const auto languages = makeLanguages();
	REQUIRE(getParsedLanguageByTag(languages, "en") != nullptr);
	CHECK(getParsedLanguageByTag(languages, "en")->percent() == 70);
	CHECK(getParsedLanguageByTag(languages, "ko") == nullptr);
}

TEST_CASE("getParsedPrimaryLanguage picks the highest share") {
	const auto languages = makeLanguages();
	REQUIRE(getParsedPrimaryLanguage(languages) != nullptr);
	CHECK(getParsedPrimaryLanguage(languages)->tag() == "en");
	CHECK(getParsedPrimaryLanguage(google::protobuf::RepeatedPtrField<lyric::parsed::LanguageItem>{}) == nullptr);
}
