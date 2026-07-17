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
		std::vector<std::string> agents;
	};

	/**
	 * Build two sample agents.
	 */
	std::vector<AgentItem> sampleAgents() {
		return {
			AgentItem{"a1", AgentType::Person, {"Alice"}},
			AgentItem{"a2", AgentType::Group,  {"Bob"}  },
		};
	}
} // namespace

TEST_CASE("getAgentById and resolveAgents preserve order and skip missing ids") {
	const auto agents = sampleAgents();
	CHECK(getAgentById(agents, "a2")->names[0] == "Bob");
	CHECK(getAgentById(agents, "x") == nullptr);

	const std::vector<std::string>      ids      = {"a2", "missing", "a1"};
	const std::vector<const AgentItem*> resolved = resolveAgents(agents, ids);
	REQUIRE(resolved.size() == 2);
	CHECK(resolved[0]->id == "a2");
	CHECK(resolved[1]->id == "a1");
}

TEST_CASE("resolveLineAgents reads agent ids from the line") {
	const auto      agents = sampleAgents();
	const AgentLine line   = {
		{"a1", "a2"}
        };
	const std::vector<const AgentItem*> resolved = resolveLineAgents(agents, line);
	REQUIRE(resolved.size() == 2);
	CHECK(resolved[0]->id == "a1");
	CHECK(resolved[1]->id == "a2");
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
	CHECK(getPrimaryAgent(agents, lines)->id == "a1");

	const std::vector<AgentLine> a2Lines = {{{"a2"}}, {{"a2"}}};
	CHECK(getPrimaryAgent(agents, a2Lines)->id == "a2");

	const std::vector<AgentLine> emptyLines = {{{}}};
	CHECK(getPrimaryAgent(agents, emptyLines) == nullptr);
}

TEST_CASE("getAgentsByType and hasAgent filter by type") {
	const auto agents  = sampleAgents();
	const auto persons = getAgentsByType(agents, AgentType::Person);
	REQUIRE(persons.size() == 1);
	CHECK(persons[0]->id == "a1");
	CHECK(hasAgent(agents, AgentType::Group) == true);
	CHECK(hasAgent(agents, AgentType::Other) == false);
}
