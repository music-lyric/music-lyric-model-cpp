#include "doctest.h"
#include "music_lyric_model.h"

using namespace music_lyric_model::runtime;
using namespace lyric::runtime;

namespace {
	/**
	 * Builds sample lyric metadata.
	 */
	Meta makeSampleMeta() {
		Meta meta;

		MetaText* ja = meta.add_titles();
		ja->set_content("歌");
		MetaText* en = meta.add_titles();
		en->set_language("en");
		en->set_content("Song");

		MetaCredit* credit = meta.add_credits();
		credit->set_role("lyricist");
		credit->add_names()->set_content("Alice");

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

TEST_CASE("meta makers preserve initial values") {
	MetaText text;
	text.set_content("Song");
	CHECK(makeMetaText(text).content() == "Song");
	Meta meta;
	*meta.add_titles() = text;
	CHECK(makeMeta(meta).titles(0).content() == "Song");
	CHECK(makeMetaCredit().role().empty());
	CHECK(makeMetaUnknown().key().empty());
	CHECK(makeMetaReference().platform().empty());
}

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
