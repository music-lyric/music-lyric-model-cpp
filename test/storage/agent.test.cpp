#include "doctest.h"
#include "music_lyric_model.h"

using namespace lyric::common;
using namespace lyric::storage;
using google::protobuf::RepeatedPtrField;

namespace {
	/**
	 * Builds two sample storage agents.
	 */
	RepeatedPtrField<AgentItem> makeAgents() {
		RepeatedPtrField<AgentItem> agents;
		AgentItem*                  first = agents.Add();
		first->set_id("a1");
		first->set_type(AGENT_TYPE_PERSON);
		first->add_names("Alice");
		AgentItem* second = agents.Add();
		second->set_id("a2");
		second->set_type(AGENT_TYPE_GROUP);
		second->add_names("Chorus");
		return agents;
	}

	/**
	 * Builds a storage line referencing the given agents.
	 */
	Line makeAgentLine(const std::vector<std::string>& ids) {
		Line line;
		for (const auto& id : ids) {
			line.mutable_content()->add_agents(id);
		}
		return music_lyric_model::storage::makeLine(line);
	}

	/**
	 * Builds sample storage lyric data with agent references.
	 */
	Lyric makeAgentLyric() {
		Lyric lyric;
		*lyric.mutable_agents() = makeAgents();
		*lyric.add_lines()      = makeAgentLine({"a1", "a2"});
		*lyric.add_lines()      = makeAgentLine({"a1"});
		return music_lyric_model::storage::makeLyric(lyric);
	}
} // namespace

TEST_CASE("storage agent maker preserves initial values") {
	AgentItem item;
	item.set_id("a1");
	CHECK(music_lyric_model::storage::makeAgentItem(item).id() == "a1");
}

TEST_CASE("storage agents resolve line references in id order") {
	const RepeatedPtrField<AgentItem> agents = makeAgents();
	const Line                        line   = makeAgentLine({"a2", "missing", "a1"});
	const auto                        result = music_lyric_model::storage::resolveLineAgents(line, agents);
	REQUIRE(result.size() == 2);
	CHECK(result.at(0)->id() == "a2");
	CHECK(result.at(1)->id() == "a1");
	CHECK(music_lyric_model::storage::getAgentById(agents, "missing") == nullptr);
}

TEST_CASE("storage agent line counts include every id reference") {
	const Lyric lyric  = makeAgentLyric();
	const auto  counts = music_lyric_model::storage::getAgentLineCounts(lyric.lines());
	CHECK(counts.at("a1") == 2);
	CHECK(counts.at("a2") == 1);
	CHECK(music_lyric_model::storage::getAgentLineCount(lyric.lines(), "a1") == 2);
	CHECK(music_lyric_model::storage::getAgentLineCount(lyric.lines(), "missing") == 0);
}

TEST_CASE("storage primary agent is the first highest count agent") {
	const Lyric lyric = makeAgentLyric();
	REQUIRE(music_lyric_model::storage::getPrimaryAgent(lyric) != nullptr);
	CHECK(music_lyric_model::storage::getPrimaryAgent(lyric)->id() == "a1");
	CHECK(music_lyric_model::storage::getPrimaryAgent(music_lyric_model::storage::makeLyric()) == nullptr);
}

TEST_CASE("storage agents filter by type") {
	const RepeatedPtrField<AgentItem> agents = makeAgents();
	CHECK(music_lyric_model::storage::getAgentsByType(agents, AGENT_TYPE_PERSON).size() == 1);
	CHECK(music_lyric_model::storage::getAgentsByType(agents, AGENT_TYPE_PERSON).at(0)->id() == "a1");
	CHECK(music_lyric_model::storage::hasAgent(agents, AGENT_TYPE_GROUP) == true);
	CHECK(music_lyric_model::storage::hasAgent(agents, AGENT_TYPE_OTHER) == false);
}
