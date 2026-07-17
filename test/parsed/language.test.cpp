#include "doctest.h"
#include "music_lyric_model.h"

using namespace music_lyric_model::parsed;

namespace {
	/**
	 * Build a sample language distribution.
	 */
	std::vector<LanguageItem> makeLanguages() {
		return {
			LanguageItem{"en", 70},
			LanguageItem{"ja", 30},
		};
	}
} // namespace

TEST_CASE("LanguageItem preserves initial values") {
	const LanguageItem item{"en", 70};
	CHECK(item.tag == "en");
	CHECK(item.percent == 70);
}

TEST_CASE("hasParsedLanguage checks presence by tag") {
	const auto languages = makeLanguages();
	CHECK(hasParsedLanguage(languages, "ja") == true);
	CHECK(hasParsedLanguage(languages, "ko") == false);
}

TEST_CASE("getParsedLanguageByTag finds the entry") {
	const auto languages = makeLanguages();
	REQUIRE(getParsedLanguageByTag(languages, "en") != nullptr);
	CHECK(getParsedLanguageByTag(languages, "en")->percent == 70);
	CHECK(getParsedLanguageByTag(languages, "ko") == nullptr);
}

TEST_CASE("getParsedPrimaryLanguage picks the highest share") {
	const auto languages = makeLanguages();
	REQUIRE(getParsedPrimaryLanguage(languages) != nullptr);
	CHECK(getParsedPrimaryLanguage(languages)->tag == "en");
	CHECK(getParsedPrimaryLanguage(std::vector<LanguageItem>{}) == nullptr);
}
