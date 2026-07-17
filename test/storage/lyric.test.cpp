#include <string>

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
	CHECK(getStorageLineText(first) == "hello");
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
