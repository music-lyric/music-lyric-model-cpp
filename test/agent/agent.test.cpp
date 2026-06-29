#include "agent/agent.h"

#include "doctest.h"
#include "line/content.h"

using namespace music_lyric_model;
using namespace lyric;
using google::protobuf::RepeatedPtrField;

namespace {
	RepeatedPtrField<AgentItem> makeAgents() {
		RepeatedPtrField<AgentItem> agents;
		AgentItem*                  a1 = agents.Add();
		a1->set_id("a1");
		a1->set_type(AGENT_TYPE_PERSON);
		a1->set_count(3);
		a1->add_names("Alice");
		AgentItem* a2 = agents.Add();
		a2->set_id("a2");
		a2->set_type(AGENT_TYPE_GROUP);
		a2->set_count(5);
		a2->add_names("Chorus");
		return agents;
	}
} // namespace

TEST_CASE("getAgentById finds the agent") {
	const RepeatedPtrField<AgentItem> agents = makeAgents();
	REQUIRE(getAgentById(agents, "a2") != nullptr);
	CHECK(getAgentById(agents, "a2")->names(0) == "Chorus");
	CHECK(getAgentById(agents, "none") == nullptr);
}

TEST_CASE("getLineAgent resolves a line reference") {
	const RepeatedPtrField<AgentItem> agents = makeAgents();
	LineNormal                        body;
	body.mutable_agent()->set_id("a2");
	const Line line = makeLineNormal(body);
	REQUIRE(getLineAgent(line, agents) != nullptr);
	CHECK(getLineAgent(line, agents)->id() == "a2");
	CHECK(getLineAgent(makeLineNormal(), agents) == nullptr);
	CHECK(getLineAgent(makeLineInterlude(), agents) == nullptr);
}

TEST_CASE("getPrimaryAgent picks the most lines") {
	const RepeatedPtrField<AgentItem> agents = makeAgents();
	REQUIRE(getPrimaryAgent(agents) != nullptr);
	CHECK(getPrimaryAgent(agents)->id() == "a2");
	CHECK(getPrimaryAgent(RepeatedPtrField<AgentItem>()) == nullptr);
}
