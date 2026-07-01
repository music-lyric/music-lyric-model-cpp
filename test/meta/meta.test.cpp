#include "meta/meta.h"

#include "doctest.h"

using namespace music_lyric_model;
using namespace lyric;

namespace {
	Meta makeSampleMeta() {
		Meta meta;

		MetaText* ja = meta.add_titles();
		ja->set_value("歌");
		MetaText* en = meta.add_titles();
		en->set_language("en");
		en->set_value("Song");

		MetaCredit* credit = meta.add_credits();
		credit->set_role("lyricist");
		credit->add_names()->set_value("Alice");

		MetaUnknown* unknown = meta.add_unknowns();
		unknown->set_key("mood");
		unknown->set_value("calm");

		MetaReference* reference = meta.add_references();
		reference->set_platform("apple");
		reference->add_ids("us-1");
		reference->add_ids("jp-2");

		return meta;
	}
} // namespace

TEST_CASE("getMetaText prefers a language match then falls back") {
	const Meta meta = makeSampleMeta();
	CHECK(getMetaText(meta.titles(), std::string("en")) == std::string("Song"));
	CHECK(getMetaText(meta.titles()) == std::string("歌"));
	CHECK(getMetaText(meta.titles(), std::string("ko")) == std::string("歌"));
	CHECK(getMetaText(meta.artists()) == std::nullopt);
}

TEST_CASE("getMetaUnknown reads values by original key") {
	const Meta meta = makeSampleMeta();
	REQUIRE(getMetaUnknown(meta.unknowns(), "mood").size() == 1);
	CHECK(getMetaUnknown(meta.unknowns(), "mood").at(0) == "calm");
	CHECK(getMetaUnknown(meta.unknowns(), "none").empty());
}

TEST_CASE("getMetaReference reads ids by platform") {
	const Meta meta = makeSampleMeta();
	CHECK(getMetaReference(meta.references(), "apple").size() == 2);
	CHECK(getMetaReference(meta.references(), "apple").at(1) == "jp-2");
	CHECK(getMetaReference(meta.references(), "none").empty());
}
