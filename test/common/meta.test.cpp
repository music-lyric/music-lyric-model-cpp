#include <optional>
#include <string>
#include <vector>

#include "doctest.h"
#include "music_lyric_model.h"

using namespace music_lyric_model::common;

TEST_CASE("getUnknownValues filters by original key") {
	const std::vector<Unknown> unknowns = {
		{"k1", "v1"},
		{"k2", "v2"},
		{"k1", "v3"},
	};

	const std::vector<std::string> values = getUnknownValues(unknowns, "k1");
	REQUIRE(values.size() == 2);
	CHECK(values[0] == "v1");
	CHECK(values[1] == "v3");
	CHECK(getUnknownValues(unknowns, "missing").empty());
}

TEST_CASE("getMetaText prefers a language match then falls back") {
	MetaText zh;
	zh.language = "zh";
	zh.content  = "title-zh";
	MetaText en;
	en.language = "en";
	en.content  = "title";
	const std::vector<MetaText> items = {zh, en};

	CHECK(*getMetaText(items, std::string("en")) == "title");
	CHECK(*getMetaText(items) == "title-zh");
	CHECK(*getMetaText(items, std::string("ko")) == "title-zh");
	CHECK(getMetaText(std::vector<MetaText>{}) == std::nullopt);
}

TEST_CASE("getMetaUnknown and getMetaReference read nested collections") {
	const std::vector<Unknown> unknowns = {{"offset", "12"}};
	CHECK(getMetaUnknown(unknowns, "offset") == std::vector<std::string>{"12"});

	MetaReference ref;
	ref.platform = "spotify";
	ref.ids      = {"id-a", "id-b"};
	const std::vector<MetaReference> references = {ref};
	CHECK(getMetaReference(references, "spotify") == std::vector<std::string>({"id-a", "id-b"}));
	CHECK(getMetaReference(references, "apple").empty());
}

TEST_CASE("Part preserves type and label") {
	const Part part{PartType::Chorus, std::optional<std::string>("Hook")};
	CHECK(part.type == PartType::Chorus);
	CHECK(part.label == "Hook");
}

TEST_CASE("getFirstAnnotation prefers a language match then falls back") {
	LineAnnotationTranslation zh;
	zh.language = "zh";
	zh.content  = "nihao";
	LineAnnotationTranslation en;
	en.language = "en";
	en.content  = "hi";
	const std::vector<LineAnnotationTranslation> items = {zh, en};

	CHECK(getFirstAnnotation(items, std::string("en"))->content == "hi");
	CHECK(getFirstAnnotation(items)->content == "nihao");
	CHECK(getFirstAnnotation(items, std::string("ko"))->content == "nihao");
	CHECK(getFirstAnnotation(std::vector<LineAnnotationTranslation>{}) == nullptr);
}
