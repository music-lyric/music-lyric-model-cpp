#include "runtime/agent.h"

#include "doctest.h"
#include "runtime/info.h"
#include "runtime/line.h"

using namespace music_lyric_model;
using namespace lyric::common;
using namespace lyric::runtime;
using google::protobuf::RepeatedPtrField;

namespace {
	/**
	 * Builds two sample agents.
	 */
	RepeatedPtrField<AgentItem> makeAgents() {
		RepeatedPtrField<AgentItem> agents;
		AgentItem*                  a1 = agents.Add();
		a1->set_id("a1");
		a1->set_type(AGENT_TYPE_PERSON);
		a1->add_names("Alice");
		AgentItem* a2 = agents.Add();
		a2->set_id("a2");
		a2->set_type(AGENT_TYPE_GROUP);
		a2->add_names("Chorus");
		return agents;
	}

	/**
	 * Builds a normal line performed by the given agent.
	 */
	Line makeAgentLine(const std::string& id) {
		LineNormal body;
		body.mutable_content()->mutable_agent()->set_id(id);
		return makeLineNormal(body);
	}

	/**
	 * Builds sample lyric info with agent-tagged lines.
	 */
	Info makeAgentInfo() {
		Info info;
		*info.mutable_agents() = makeAgents();
		*info.add_lines()      = makeAgentLine("a2");
		*info.add_lines()      = makeAgentLine("a2");
		*info.add_lines()      = makeAgentLine("a1");
		return info;
	}
} // namespace

TEST_CASE("agent makers preserve initial values") {
	AgentItem item;
	item.set_id("a1");
	CHECK(makeAgentItem(item).id() == "a1");
	LineAgent reference;
	reference.set_id("a1");
	CHECK(makeLineAgent(reference).id() == "a1");
}

TEST_CASE("getAgentById finds the agent") {
	const RepeatedPtrField<AgentItem> agents = makeAgents();
	REQUIRE(getAgentById(agents, "a2") != nullptr);
	CHECK(getAgentById(agents, "a2")->names(0) == "Chorus");
	CHECK(getAgentById(agents, "none") == nullptr);
}

TEST_CASE("resolveLineAgent resolves a line reference") {
	const RepeatedPtrField<AgentItem> agents = makeAgents();
	const Line                        line   = makeAgentLine("a2");
	REQUIRE(resolveLineAgent(line, agents) != nullptr);
	CHECK(resolveLineAgent(line, agents)->id() == "a2");
	CHECK(resolveLineAgent(makeLineNormal(), agents) == nullptr);
	CHECK(resolveLineAgent(makeLineInterlude(), agents) == nullptr);
}

TEST_CASE("getAgentLineCounts and getAgentLineCount tally lines per agent") {
	const Info info = makeAgentInfo();
	CHECK(getAgentLineCounts(info.lines()).at("a2") == 2);
	CHECK(getAgentLineCount(info.lines(), "a1") == 1);
	CHECK(getAgentLineCount(info.lines(), "none") == 0);
}

TEST_CASE("getAgentLineIndexes tracks global and block occurrence") {
	const Info info     = makeAgentInfo();
	const auto indexes  = getAgentLineIndexes(info.lines());
	const auto secondA2 = indexes.at(&info.lines(1));
	const auto firstA1  = indexes.at(&info.lines(2));
	CHECK(secondA2.globalIndex == 1);
	CHECK(secondA2.blockIndex == 1);
	CHECK(firstA1.globalIndex == 0);
	CHECK(firstA1.blockIndex == 0);
}

TEST_CASE("getPrimaryAgent picks the most lines") {
	const Info info = makeAgentInfo();
	REQUIRE(getPrimaryAgent(info) != nullptr);
	CHECK(getPrimaryAgent(info)->id() == "a2");
	CHECK(getPrimaryAgent(makeInfo()) == nullptr);
}

TEST_CASE("getAgentsByType and hasAgent filter by type") {
	const RepeatedPtrField<AgentItem> agents = makeAgents();
	CHECK(getAgentsByType(agents, AGENT_TYPE_PERSON).size() == 1);
	CHECK(getAgentsByType(agents, AGENT_TYPE_PERSON).at(0)->id() == "a1");
	CHECK(hasAgent(agents, AGENT_TYPE_GROUP) == true);
	CHECK(hasAgent(agents, AGENT_TYPE_OTHER) == false);
}
