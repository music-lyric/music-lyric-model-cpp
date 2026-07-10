#include <algorithm>

#include "doctest.h"
#include "music_lyric_model.h"

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
		*content->add_words() = music_lyric_model::storage::makeWordNormal(word);
		return music_lyric_model::storage::makeLine(line);
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
		return music_lyric_model::storage::makeLyric(lyric);
	}
} // namespace

TEST_CASE("storage proto namespace exports generated types") {
	music_lyric_model::storage::proto::Lyric model;
	model.set_timing(lyric::common::TIMING_WORD);
	CHECK(model.timing() == lyric::common::TIMING_WORD);
}

TEST_CASE("makeLyric preserves initial values and stamps the schema version") {
	Lyric init;
	init.set_version("0.0.1");
	init.set_timing(lyric::common::TIMING_WORD);
	const Lyric result = music_lyric_model::storage::makeLyric(init);
	CHECK(result.version() == music_lyric_model::SCHEMA_VERSION);
	CHECK(result.timing() == lyric::common::TIMING_WORD);
}

TEST_CASE("storage lyric line helpers and multi-agent resolution preserve order") {
	const Lyric lyric = buildLyric();
	const Line& first = lyric.lines(0);
	REQUIRE(music_lyric_model::storage::getLineTime(first) != nullptr);
	CHECK(music_lyric_model::storage::getLineTime(first)->start() == 1000);
	CHECK(music_lyric_model::storage::getLineText(first) == "hello");
	const auto agents = music_lyric_model::storage::resolveLineAgents(first, lyric.agents());
	REQUIRE(agents.size() == 2);
	CHECK(agents.at(0)->id() == "a1");
	CHECK(agents.at(1)->id() == "a2");
}

TEST_CASE("storage lyric agent line counts use string references") {
	const Lyric lyric  = buildLyric();
	const auto  counts = music_lyric_model::storage::getAgentLineCounts(lyric.lines());
	CHECK(counts.at("a1") == 2);
	CHECK(counts.at("a2") == 1);
	REQUIRE(music_lyric_model::storage::getPrimaryAgent(lyric) != nullptr);
	CHECK(music_lyric_model::storage::getPrimaryAgent(lyric)->id() == "a1");
}

TEST_CASE("storage lyric binary and json round-trips preserve content") {
	const Lyric lyric      = buildLyric();
	const Lyric fromBinary = music_lyric_model::storage::decodeLyric(music_lyric_model::storage::encodeLyric(lyric));
	CHECK(music_lyric_model::storage::getLineText(fromBinary.lines(0)) == "hello");
	const Lyric fromJson = music_lyric_model::storage::lyricFromJson(music_lyric_model::storage::lyricToJson(lyric));
	CHECK(music_lyric_model::storage::getLineText(fromJson.lines(1)) == "world");
}

TEST_CASE("storage lyric sorting orders lines and backgrounds by start time") {
	Lyric lyric = buildLyric();
	std::reverse(lyric.mutable_lines()->begin(), lyric.mutable_lines()->end());
	LineContent* late = lyric.mutable_lines(0)->add_backgrounds();
	late->mutable_time()->set_start(2500);
	LineContent* early = lyric.mutable_lines(0)->add_backgrounds();
	early->mutable_time()->set_start(2100);
	music_lyric_model::storage::sortLinesByTime(lyric);
	REQUIRE(music_lyric_model::storage::getLineTime(lyric.lines(0)) != nullptr);
	CHECK(music_lyric_model::storage::getLineTime(lyric.lines(0))->start() == 1000);
	CHECK(lyric.lines(1).backgrounds(0).time().start() == 2100);
	CHECK(lyric.lines(1).backgrounds(1).time().start() == 2500);
}
