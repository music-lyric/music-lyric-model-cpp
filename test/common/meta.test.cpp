#include <optional>
#include <string>
#include <vector>

#include "doctest.h"
#include "music_lyric_model.h"

using namespace music_lyric_model::common;

TEST_CASE("getUnknownValues filters by original key") {
	google::protobuf::RepeatedPtrField<lyric::common::Unknown> unknowns;
	lyric::common::Unknown                                     a;
	a.set_key("k1");
	a.set_value("v1");
	*unknowns.Add() = makeUnknown(a);
	lyric::common::Unknown b;
	b.set_key("k2");
	b.set_value("v2");
	*unknowns.Add() = makeUnknown(b);
	lyric::common::Unknown c;
	c.set_key("k1");
	c.set_value("v3");
	*unknowns.Add() = makeUnknown(c);

	const std::vector<std::string> values = getUnknownValues(unknowns, "k1");
	REQUIRE(values.size() == 2);
	CHECK(values[0] == "v1");
	CHECK(values[1] == "v3");
	CHECK(getUnknownValues(unknowns, "missing").empty());
}

TEST_CASE("getMetaText prefers a language match then falls back") {
	google::protobuf::RepeatedPtrField<lyric::common::MetaText> items;
	lyric::common::MetaText                                     zh;
	zh.set_language("zh");
	zh.set_content("标题");
	*items.Add() = makeMetaText(zh);
	lyric::common::MetaText en;
	en.set_language("en");
	en.set_content("title");
	*items.Add() = makeMetaText(en);

	CHECK(*getMetaText(items, std::string("en")) == "title");
	CHECK(*getMetaText(items) == "标题");
	CHECK(*getMetaText(items, std::string("ko")) == "标题");
	CHECK(getMetaText(google::protobuf::RepeatedPtrField<lyric::common::MetaText>{}) == std::nullopt);
}

TEST_CASE("getMetaUnknown and getMetaReference read nested collections") {
	google::protobuf::RepeatedPtrField<lyric::common::Unknown> unknowns;
	lyric::common::Unknown                                     unknown;
	unknown.set_key("offset");
	unknown.set_value("12");
	*unknowns.Add() = makeUnknown(unknown);
	CHECK(getMetaUnknown(unknowns, "offset") == std::vector<std::string>{"12"});

	google::protobuf::RepeatedPtrField<lyric::common::MetaReference> references;
	lyric::common::MetaReference                                     ref;
	ref.set_platform("spotify");
	ref.add_ids("id-a");
	ref.add_ids("id-b");
	*references.Add() = makeMetaReference(ref);
	CHECK(getMetaReference(references, "spotify") == std::vector<std::string>({"id-a", "id-b"}));
	CHECK(getMetaReference(references, "apple").empty());
}

TEST_CASE("makePart preserves type and label") {
	lyric::common::Part init;
	init.set_type(lyric::common::PART_TYPE_CHORUS);
	init.set_label("Hook");
	const lyric::common::Part part = makePart(init);
	CHECK(part.type() == lyric::common::PART_TYPE_CHORUS);
	CHECK(part.label() == "Hook");
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
	CHECK(getFirstAnnotation(google::protobuf::RepeatedPtrField<lyric::common::LineAnnotationTranslation>{}) == nullptr);
}
