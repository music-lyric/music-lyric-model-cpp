#include <string>
#include <unordered_map>
#include <vector>

#include "doctest.h"
#include "music_lyric_model.h"

using namespace music_lyric_model::common;

namespace {
	/**
	 * Line-like fixture exposing agent id references.
	 */
	struct AgentLine {
		std::vector<std::string> ids;

		/**
		 * Agent id references, matching the repeated field name used by proto lines.
		 */
		const std::vector<std::string>& agents() const {
			return ids;
		}
	};

	/**
	 * Build two sample agents.
	 */
	google::protobuf::RepeatedPtrField<lyric::common::AgentItem> sampleAgents() {
		google::protobuf::RepeatedPtrField<lyric::common::AgentItem> agents;
		lyric::common::AgentItem                                    a1;
		a1.set_id("a1");
		a1.set_type(lyric::common::AGENT_TYPE_PERSON);
		a1.add_names("Alice");
		*agents.Add() = makeAgentItem(a1);
		lyric::common::AgentItem a2;
		a2.set_id("a2");
		a2.set_type(lyric::common::AGENT_TYPE_GROUP);
		a2.add_names("Bob");
		*agents.Add() = makeAgentItem(a2);
		return agents;
	}
} // namespace

TEST_CASE("getAgentById and resolveAgents preserve order and skip missing ids") {
	const auto agents = sampleAgents();
	CHECK(getAgentById(agents, "a2")->names(0) == "Bob");
	CHECK(getAgentById(agents, "x") == nullptr);

	const std::vector<std::string>                     ids      = {"a2", "missing", "a1"};
	const std::vector<const lyric::common::AgentItem*> resolved = resolveAgents(agents, ids);
	REQUIRE(resolved.size() == 2);
	CHECK(resolved[0]->id() == "a2");
	CHECK(resolved[1]->id() == "a1");
}

TEST_CASE("resolveLineAgents reads agent ids from the line") {
	const auto                                         agents   = sampleAgents();
	const AgentLine                                    line     = {{"a1", "a2"}};
	const std::vector<const lyric::common::AgentItem*> resolved = resolveLineAgents(agents, line);
	REQUIRE(resolved.size() == 2);
	CHECK(resolved[0]->id() == "a1");
	CHECK(resolved[1]->id() == "a2");
}

TEST_CASE("getAgentLineCounts counts each id once per line") {
	const std::vector<AgentLine>                    lines  = {{{"a1", "a2"}}, {{"a1"}}, {{"a1", "a1"}}};
	const std::unordered_map<std::string, uint32_t> counts = getAgentLineCounts(lines);
	CHECK(counts.at("a1") == 3);
	CHECK(counts.at("a2") == 1);
}

TEST_CASE("getPrimaryAgent picks the agent on the most lines") {
	const auto                   agents = sampleAgents();
	const std::vector<AgentLine> lines  = {{{"a1", "a2"}}, {{"a1"}}, {{"a1", "a1"}}};
	CHECK(getPrimaryAgent(agents, lines)->id() == "a1");

	const std::vector<AgentLine> a2Lines = {{{"a2"}}, {{"a2"}}};
	CHECK(getPrimaryAgent(agents, a2Lines)->id() == "a2");

	const std::vector<AgentLine> emptyLines = {{{}}};
	CHECK(getPrimaryAgent(agents, emptyLines) == nullptr);
}

TEST_CASE("getAgentsByType and hasAgent filter by type") {
	const auto agents = sampleAgents();
	const auto persons = getAgentsByType(agents, lyric::common::AGENT_TYPE_PERSON);
	REQUIRE(persons.size() == 1);
	CHECK(persons[0]->id() == "a1");
	CHECK(hasAgent(agents, lyric::common::AGENT_TYPE_GROUP) == true);
	CHECK(hasAgent(agents, lyric::common::AGENT_TYPE_OTHER) == false);
}
