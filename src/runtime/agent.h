#ifndef MUSIC_LYRIC_RUNTIME_AGENT_H_
#define MUSIC_LYRIC_RUNTIME_AGENT_H_

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/agent.pb.h"
#include "runtime/agent/agent.pb.h"
#include "runtime/info.pb.h"
#include "runtime/line/content.pb.h"

namespace music_lyric_model {
	using namespace lyric::common;
	using namespace lyric::runtime;

	/**
	 * Occurrence index of a line's performing agent, computed from the current line order.
	 */
	struct AgentLineIndex {
		/**
		 * Occurrence of this agent among all lines, 0-based.
		 */
		uint32_t globalIndex;

		/**
		 * Occurrence within the current run of consecutive same-agent lines, 0-based.
		 */
		uint32_t blockIndex;
	};

	/**
	 * Creates an AgentItem.
	 */
	lyric::runtime::AgentItem makeAgentItem(const lyric::runtime::AgentItem& item = {});

	/**
	 * Creates a LineAgent, a line's reference to an agent.
	 */
	lyric::runtime::LineAgent makeLineAgent(const lyric::runtime::LineAgent& agent = {});

	/**
	 * Agent with the given id, null when absent.
	 */
	const lyric::runtime::AgentItem* getAgentById(const google::protobuf::RepeatedPtrField<lyric::runtime::AgentItem>& agents, const std::string& id);

	/**
	 * Agent performing a line, resolved from its reference.
	 */
	const lyric::runtime::AgentItem* resolveLineAgent(const lyric::runtime::Line& line, const google::protobuf::RepeatedPtrField<lyric::runtime::AgentItem>& agents);

	/**
	 * Number of lines each agent performs, keyed by agent id.
	 */
	std::unordered_map<std::string, uint32_t> getAgentLineCounts(const google::protobuf::RepeatedPtrField<lyric::runtime::Line>& lines);

	/**
	 * Number of lines the given agent performs.
	 */
	uint32_t getAgentLineCount(const google::protobuf::RepeatedPtrField<lyric::runtime::Line>& lines, const std::string& id);

	/**
	 * Global and block occurrence index of each line's agent, keyed by the line.
	 */
	std::unordered_map<const lyric::runtime::Line*, AgentLineIndex> getAgentLineIndexes(const google::protobuf::RepeatedPtrField<lyric::runtime::Line>& lines);

	/**
	 * Agent performing the most lines, null when empty.
	 */
	const lyric::runtime::AgentItem* getPrimaryAgent(const lyric::runtime::Info& info);

	/**
	 * All agents of a type.
	 */
	std::vector<const lyric::runtime::AgentItem*> getAgentsByType(const google::protobuf::RepeatedPtrField<lyric::runtime::AgentItem>& agents, lyric::common::AgentType type);

	/**
	 * Whether any agent of a type exists.
	 */
	bool hasAgent(const google::protobuf::RepeatedPtrField<lyric::runtime::AgentItem>& agents, lyric::common::AgentType type);
} // namespace music_lyric_model

#endif
