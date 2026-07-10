#include "runtime/language.h"

#include "doctest.h"

using namespace music_lyric_model;
using namespace lyric::runtime;
using google::protobuf::RepeatedPtrField;

namespace {
	/**
	 * Builds a sample language distribution.
	 */
	RepeatedPtrField<LanguageItem> makeLanguages() {
		RepeatedPtrField<LanguageItem> languages;
		LanguageItem*                  en = languages.Add();
		en->set_tag("en");
		en->set_percent(70);
		LanguageItem* ja = languages.Add();
		ja->set_tag("ja");
		ja->set_percent(30);
		return languages;
	}
} // namespace

TEST_CASE("makeLanguageItem preserves initial values") {
	LanguageItem init;
	init.set_tag("en");
	init.set_percent(70);
	const LanguageItem item = makeLanguageItem(init);
	CHECK(item.tag() == "en");
	CHECK(item.percent() == 70);
}

TEST_CASE("hasLanguage checks presence by tag") {
	const RepeatedPtrField<LanguageItem> languages = makeLanguages();
	CHECK(hasLanguage(languages, "ja") == true);
	CHECK(hasLanguage(languages, "ko") == false);
}

TEST_CASE("getLanguageByTag finds the entry") {
	const RepeatedPtrField<LanguageItem> languages = makeLanguages();
	REQUIRE(getLanguageByTag(languages, "en") != nullptr);
	CHECK(getLanguageByTag(languages, "en")->percent() == 70);
	CHECK(getLanguageByTag(languages, "ko") == nullptr);
}

TEST_CASE("getPrimaryLanguage picks the highest share") {
	const RepeatedPtrField<LanguageItem> languages = makeLanguages();
	REQUIRE(getPrimaryLanguage(languages) != nullptr);
	CHECK(getPrimaryLanguage(languages)->tag() == "en");
	CHECK(getPrimaryLanguage(RepeatedPtrField<LanguageItem>()) == nullptr);
}
