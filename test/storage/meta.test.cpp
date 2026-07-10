#include "doctest.h"
#include "music_lyric_model.h"

using namespace lyric::storage;

namespace {
	/**
	 * Builds sample storage metadata.
	 */
	Meta makeSampleMeta() {
		Meta meta;
		meta.add_titles()->set_content("歌");
		MetaText* english = meta.add_titles();
		english->set_language("en");
		english->set_content("Song");
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
		return music_lyric_model::storage::makeMeta(meta);
	}
} // namespace

TEST_CASE("storage meta makers preserve initial values") {
	MetaText text;
	text.set_content("Song");
	CHECK(music_lyric_model::storage::makeMetaText(text).content() == "Song");
	MetaCredit credit;
	credit.set_role("lyricist");
	CHECK(music_lyric_model::storage::makeMetaCredit(credit).role() == "lyricist");
	MetaUnknown unknown;
	unknown.set_key("mood");
	CHECK(music_lyric_model::storage::makeMetaUnknown(unknown).key() == "mood");
	MetaReference reference;
	reference.set_platform("apple");
	CHECK(music_lyric_model::storage::makeMetaReference(reference).platform() == "apple");
}

TEST_CASE("storage meta text prefers a language match then falls back") {
	const Meta meta = makeSampleMeta();
	CHECK(music_lyric_model::storage::getMetaText(meta.titles(), std::string("en")) == std::string("Song"));
	CHECK(music_lyric_model::storage::getMetaText(meta.titles()) == std::string("歌"));
	CHECK(music_lyric_model::storage::getMetaText(meta.titles(), std::string("ko")) == std::string("歌"));
	CHECK(music_lyric_model::storage::getMetaText(meta.artists()) == std::nullopt);
}

TEST_CASE("storage unknown metadata is filtered by original key") {
	const Meta meta = makeSampleMeta();
	REQUIRE(music_lyric_model::storage::getMetaUnknown(meta.unknowns(), "mood").size() == 1);
	CHECK(music_lyric_model::storage::getMetaUnknown(meta.unknowns(), "mood").at(0) == "calm");
	CHECK(music_lyric_model::storage::getMetaUnknown(meta.unknowns(), "missing").empty());
}

TEST_CASE("storage metadata references are selected by platform") {
	const Meta meta = makeSampleMeta();
	CHECK(music_lyric_model::storage::getMetaReference(meta.references(), "apple").size() == 2);
	CHECK(music_lyric_model::storage::getMetaReference(meta.references(), "apple").at(1) == "jp-2");
	CHECK(music_lyric_model::storage::getMetaReference(meta.references(), "missing").empty());
}
