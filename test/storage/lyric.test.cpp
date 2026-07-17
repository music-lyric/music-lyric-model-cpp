#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

#include "doctest.h"
#include "music_lyric_model.h"

using namespace music_lyric_model;
using namespace music_lyric_model::common;
using namespace music_lyric_model::storage;

namespace {
	/**
	 * Build a Lyric with two lines referencing agents by id.
	 */
	lyric::storage::Lyric buildLyric() {
		lyric::storage::Lyric init;

		lyric::common::AgentItem a1;
		a1.set_id("a1");
		a1.add_names("Alice");
		*init.add_agents() = makeAgentItem(a1);

		lyric::common::AgentItem a2;
		a2.set_id("a2");
		a2.add_names("Bob");
		*init.add_agents() = makeAgentItem(a2);

		lyric::storage::Line first;
		first.mutable_time()->set_start(1000);
		first.mutable_time()->set_end(2000);
		first.add_agents("a1");
		first.add_agents("a2");
		lyric::common::WordNormal hello;
		hello.set_content("hello");
		hello.set_language("en");
		*first.add_words() = makeWordNormal(hello);
		*init.add_lines() = makeStorageLine(first);

		lyric::storage::Line second;
		second.mutable_time()->set_start(2000);
		second.mutable_time()->set_end(3000);
		second.add_agents("a1");
		lyric::common::WordNormal world;
		world.set_content("world");
		*second.add_words() = makeWordNormal(world);
		*init.add_lines() = makeStorageLine(second);

		return makeStorageLyric(init);
	}
} // namespace

TEST_CASE("storage proto namespace exports generated types") {
	music_lyric_model::storage::proto::Lyric lyric;
	lyric.set_version("x");
	CHECK(lyric.version() == "x");
}

TEST_CASE("makeStorageLyric stamps the schema version") {
	CHECK(std::string(makeStorageLyric().version()) == SCHEMA_VERSION);
	lyric::storage::Lyric init;
	init.set_version("0.0.1");
	CHECK(std::string(makeStorageLyric(init).version()) == SCHEMA_VERSION);
}

TEST_CASE("line time, plain text and multi-agent resolution") {
	const lyric::storage::Lyric lyric = buildLyric();
	const lyric::storage::Line& first = lyric.lines(0);
	CHECK(getStorageLineTime(first)->start() == 1000);
	CHECK(getStorageLineDuration(first) == 1000);
	CHECK(getStorageLineText(first) == "hello");
	CHECK(getStorageLineLanguages(first) == std::vector<std::string>{"en"});
	const auto agents = resolveLineAgents(lyric.agents(), first);
	REQUIRE(agents.size() == 2);
	CHECK(agents[0]->id() == "a1");
	CHECK(agents[1]->id() == "a2");
}

TEST_CASE("agent line counts and primary agent") {
	const lyric::storage::Lyric lyric  = buildLyric();
	const auto                  counts = getAgentLineCounts(lyric.lines());
	CHECK(counts.at("a1") == 2);
	CHECK(counts.at("a2") == 1);
	CHECK(getPrimaryAgent(lyric.agents(), lyric.lines())->id() == "a1");
}

TEST_CASE("binary and json round-trip preserve content") {
	const lyric::storage::Lyric lyric      = buildLyric();
	const lyric::storage::Lyric fromBinary = decodeStorageLyric(encodeStorageLyric(lyric));
	CHECK(getStorageLineText(fromBinary.lines(0)) == "hello");
	const lyric::storage::Lyric fromJson = storageLyricFromJson(storageLyricToJson(lyric));
	CHECK(getStorageLineText(fromJson.lines(1)) == "world");
}

TEST_CASE("sortStorageLinesByTime orders ascending including backgrounds") {
	lyric::storage::Lyric lyric = buildLyric();

	lyric::storage::Line late;
	late.mutable_time()->set_start(4000);
	late.mutable_time()->set_end(5000);
	lyric::storage::LineBackground bgLate;
	bgLate.mutable_time()->set_start(4500);
	bgLate.mutable_time()->set_end(4600);
	lyric::storage::LineBackground bgEarly;
	bgEarly.mutable_time()->set_start(4100);
	bgEarly.mutable_time()->set_end(4200);
	*late.add_backgrounds() = makeStorageLineBackground(bgLate);
	*late.add_backgrounds() = makeStorageLineBackground(bgEarly);
	*lyric.add_lines()      = makeStorageLine(late);

	std::reverse(lyric.mutable_lines()->begin(), lyric.mutable_lines()->end());
	sortStorageLinesByTime(lyric);
	CHECK(getStorageLineTime(lyric.lines(0))->start() == 1000);
	CHECK(getStorageLineTime(lyric.lines(2))->start() == 4000);
	REQUIRE(lyric.lines(2).backgrounds_size() == 2);
	CHECK(lyric.lines(2).backgrounds(0).time().start() == 4100);
	CHECK(lyric.lines(2).backgrounds(1).time().start() == 4500);
}

TEST_CASE("getStorageActiveLine picks the timed line covering the moment") {
	const lyric::storage::Lyric lyric = buildLyric();
	CHECK(getStorageActiveLineIndex(lyric.lines(), 1500) == 0);
	CHECK(getStorageActiveLine(lyric.lines(), 2500) == &lyric.lines(1));
	CHECK(getStorageActiveLineIndex(lyric.lines(), 50) == -1);
	CHECK(getStorageActiveLine(lyric.lines(), 50) == nullptr);
}

TEST_CASE("getStorageLineTranslation and getStorageLineRoman read line annotations") {
	lyric::common::LineAnnotationTranslation translation;
	translation.set_language("en");
	translation.set_content("hello");
	lyric::common::LineAnnotationRoman roman;
	roman.set_language("romaji");
	roman.set_content("konnichiwa");
	lyric::common::LineAnnotation annotation;
	*annotation.add_translations() = makeLineAnnotationTranslation(translation);
	*annotation.add_romans()       = makeLineAnnotationRoman(roman);

	lyric::storage::Line line;
	*line.mutable_annotation() = makeLineAnnotation(annotation);
	CHECK(*getStorageLineTranslation(line, std::string("en")) == "hello");
	CHECK(*getStorageLineRoman(line, std::string("romaji")) == "konnichiwa");
}

TEST_CASE("decodeStorageLyric throws on invalid binary and json") {
	const std::vector<uint8_t> badBinary = {0x01, 0x02, 0x03};
	CHECK_THROWS_AS(decodeStorageLyric(badBinary), std::runtime_error);
	CHECK_THROWS_AS(storageLyricFromJson("{not-json"), std::runtime_error);
}
