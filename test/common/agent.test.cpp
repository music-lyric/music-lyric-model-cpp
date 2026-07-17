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
} // namespace

TEST_CASE("getAgentById and resolveAgents preserve order and skip missing ids") {
	google::protobuf::RepeatedPtrField<lyric::common::AgentItem> agents;
	*agents.Add() = makeAgentItem([] {
		lyric::common::AgentItem item;
		item.set_id("a1");
		item.add_names("Alice");
		return item;
	}());
	*agents.Add() = makeAgentItem([] {
		lyric::common::AgentItem item;
		item.set_id("a2");
		item.add_names("Bob");
		return item;
	}());

	CHECK(getAgentById(agents, "a2")->names(0) == "Bob");
	CHECK(getAgentById(agents, "x") == nullptr);

	const std::vector<std::string>               ids    = {"a2", "missing", "a1"};
	const std::vector<const lyric::common::AgentItem*> resolved = resolveAgents(agents, ids);
	REQUIRE(resolved.size() == 2);
	CHECK(resolved[0]->id() == "a2");
	CHECK(resolved[1]->id() == "a1");
}

TEST_CASE("resolveLineAgents reads agent ids from the line") {
	google::protobuf::RepeatedPtrField<lyric::common::AgentItem> agents;
	*agents.Add() = makeAgentItem([] {
		lyric::common::AgentItem item;
		item.set_id("a1");
		return item;
	}());
	*agents.Add() = makeAgentItem([] {
		lyric::common::AgentItem item;
		item.set_id("a2");
		return item;
	}());

	const AgentLine                              line     = {{"a1", "a2"}};
	const std::vector<const lyric::common::AgentItem*> resolved = resolveLineAgents(agents, line);
	REQUIRE(resolved.size() == 2);
	CHECK(resolved[0]->id() == "a1");
	CHECK(resolved[1]->id() == "a2");
}

TEST_CASE("getAgentLineCounts counts each id once per line") {
	const std::vector<AgentLine> lines = {
	    {{"a1", "a2"}},
	    {{"a1"}},
	    {{"a1", "a1"}},
	};
	const std::unordered_map<std::string, uint32_t> counts = getAgentLineCounts(lines);
	CHECK(counts.at("a1") == 3);
	CHECK(counts.at("a2") == 1);
}

TEST_CASE("getPrimaryAgent picks the agent on the most lines") {
	google::protobuf::RepeatedPtrField<lyric::common::AgentItem> agents;
	*agents.Add() = makeAgentItem([] {
		lyric::common::AgentItem item;
		item.set_id("a1");
		return item;
	}());
	*agents.Add() = makeAgentItem([] {
		lyric::common::AgentItem item;
		item.set_id("a2");
		return item;
	}());

	const std::vector<AgentLine> lines = {
	    {{"a1", "a2"}},
	    {{"a1"}},
	    {{"a1", "a1"}},
	};
	CHECK(getPrimaryAgent(agents, lines)->id() == "a1");

	const std::vector<AgentLine> a2Lines = {{{"a2"}}, {{"a2"}}};
	CHECK(getPrimaryAgent(agents, a2Lines)->id() == "a2");

	const std::vector<AgentLine> emptyLines = {{{}}};
	CHECK(getPrimaryAgent(agents, emptyLines) == nullptr);
}
