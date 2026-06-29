#include "meta/meta.h"

#include "doctest.h"

using namespace music_lyric_model;
using namespace lyric;
using google::protobuf::RepeatedPtrField;

namespace {
	RepeatedPtrField<MetaItem> makeMetas() {
		RepeatedPtrField<MetaItem> metas;
		MetaItem*                  ti = metas.Add();
		ti->set_key("ti");
		ti->set_title("Song");
		MetaItem* ar1 = metas.Add();
		ar1->set_key("ar");
		ar1->set_singer("Alice");
		MetaItem* ar2 = metas.Add();
		ar2->set_key("ar");
		ar2->set_singer("Bob");
		MetaItem* offset = metas.Add();
		offset->set_key("offset");
		offset->set_offset(120);
		return metas;
	}
} // namespace

TEST_CASE("hasMeta checks presence by case") {
	const RepeatedPtrField<MetaItem> metas = makeMetas();
	CHECK(hasMeta(metas, MetaItem::kTitle) == true);
	CHECK(hasMeta(metas, MetaItem::kAlbum) == false);
}

TEST_CASE("getFirstMeta reads the typed value") {
	const RepeatedPtrField<MetaItem> metas = makeMetas();
	REQUIRE(getFirstMeta(metas, MetaItem::kTitle) != nullptr);
	CHECK(getFirstMeta(metas, MetaItem::kTitle)->title() == "Song");
	REQUIRE(getFirstMeta(metas, MetaItem::kOffset) != nullptr);
	CHECK(getFirstMeta(metas, MetaItem::kOffset)->offset() == 120);
	CHECK(getFirstMeta(metas, MetaItem::kAlbum) == nullptr);
}

TEST_CASE("getAllMeta returns every entry of a case") {
	const RepeatedPtrField<MetaItem> metas = makeMetas();
	CHECK(getAllMeta(metas, MetaItem::kSinger).size() == 2);
}

TEST_CASE("getMetaByKey groups by original key") {
	const RepeatedPtrField<MetaItem> metas = makeMetas();
	CHECK(getMetaByKey(metas, "ar").size() == 2);
}
