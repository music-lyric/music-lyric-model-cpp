#include <algorithm>

#include "doctest.h"
#include "storage/storage.h"
#include "version.h"

namespace Storage = music_lyric_model::storage;

using namespace lyric::storage;

namespace {
	/**
	 * Builds a storage line with time, agent ids and one word.
	 */
	Line makeLyricLine(uint32_t start, uint32_t end, const std::vector<std::string>& agents, const std::string& text) {
		WordNormal word;
		word.set_content(text);
		Line         line;
		LineContent* content = line.mutable_content();
		content->mutable_time()->set_start(start);
		content->mutable_time()->set_end(end);
		for (const auto& id : agents) {
			content->add_agents(id);
		}
		*content->add_words() = Storage::makeWordNormal(word);
		return Storage::makeLine(line);
	}

	/**
	 * Builds sample storage lyric data for serialization and sorting tests.
	 */
	Lyric buildLyric() {
		Lyric      lyric;
		AgentItem* first = lyric.add_agents();
		first->set_id("a1");
		first->add_names("Alice");
		AgentItem* second = lyric.add_agents();
		second->set_id("a2");
		second->add_names("Bob");
		*lyric.add_lines() = makeLyricLine(1000, 2000, {"a1", "a2"}, "hello");
		*lyric.add_lines() = makeLyricLine(2000, 3000, {"a1"}, "world");
		return Storage::makeLyric(lyric);
	}
} // namespace

TEST_CASE("makeLyric preserves initial values and stamps the schema version") {
	Lyric init;
	init.set_version("0.0.1");
	init.set_timing(lyric::common::TIMING_WORD);
	const Lyric result = Storage::makeLyric(init);
	CHECK(result.version() == music_lyric_model::SCHEMA_VERSION);
	CHECK(result.timing() == lyric::common::TIMING_WORD);
}

TEST_CASE("storage lyric line helpers and multi-agent resolution preserve order") {
	const Lyric lyric = buildLyric();
	const Line& first = lyric.lines(0);
	REQUIRE(Storage::getLineTime(first) != nullptr);
	CHECK(Storage::getLineTime(first)->start() == 1000);
	CHECK(Storage::getLineText(first) == "hello");
	const auto agents = Storage::resolveLineAgents(first, lyric.agents());
	REQUIRE(agents.size() == 2);
	CHECK(agents.at(0)->id() == "a1");
	CHECK(agents.at(1)->id() == "a2");
}

TEST_CASE("storage lyric agent line counts use string references") {
	const Lyric lyric  = buildLyric();
	const auto  counts = Storage::getAgentLineCounts(lyric.lines());
	CHECK(counts.at("a1") == 2);
	CHECK(counts.at("a2") == 1);
	REQUIRE(Storage::getPrimaryAgent(lyric) != nullptr);
	CHECK(Storage::getPrimaryAgent(lyric)->id() == "a1");
}

TEST_CASE("storage lyric binary and json round-trips preserve content") {
	const Lyric lyric      = buildLyric();
	const Lyric fromBinary = Storage::decodeLyric(Storage::encodeLyric(lyric));
	CHECK(Storage::getLineText(fromBinary.lines(0)) == "hello");
	const Lyric fromJson = Storage::lyricFromJson(Storage::lyricToJson(lyric));
	CHECK(Storage::getLineText(fromJson.lines(1)) == "world");
}

TEST_CASE("storage lyric sorting orders lines and backgrounds by start time") {
	Lyric lyric = buildLyric();
	std::reverse(lyric.mutable_lines()->begin(), lyric.mutable_lines()->end());
	LineContent* late = lyric.mutable_lines(0)->add_backgrounds();
	late->mutable_time()->set_start(2500);
	LineContent* early = lyric.mutable_lines(0)->add_backgrounds();
	early->mutable_time()->set_start(2100);
	Storage::sortLinesByTime(lyric);
	REQUIRE(Storage::getLineTime(lyric.lines(0)) != nullptr);
	CHECK(Storage::getLineTime(lyric.lines(0))->start() == 1000);
	CHECK(lyric.lines(1).backgrounds(0).time().start() == 2100);
	CHECK(lyric.lines(1).backgrounds(1).time().start() == 2500);
}
