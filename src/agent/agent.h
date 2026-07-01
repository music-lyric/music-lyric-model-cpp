#ifndef MUSIC_LYRIC_AGENT_AGENT_H_
#define MUSIC_LYRIC_AGENT_AGENT_H_

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "agent/agent.pb.h"
#include "info.pb.h"
#include "line/content.pb.h"

namespace music_lyric_model {
	using namespace lyric;

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
	lyric::AgentItem makeAgentItem();

	/**
	 * Creates a LineAgent, a line's reference to an agent.
	 */
	lyric::LineAgent makeLineAgent();

	/**
	 * Agent with the given id, null when absent.
	 */
	const lyric::AgentItem* getAgentById(const google::protobuf::RepeatedPtrField<lyric::AgentItem>& agents, const std::string& id);

	/**
	 * Agent performing a line, resolved from its reference.
	 */
	const lyric::AgentItem* getLineAgent(const lyric::Line& line, const google::protobuf::RepeatedPtrField<lyric::AgentItem>& agents);

	/**
	 * Number of lines each agent performs, keyed by agent id.
	 */
	std::unordered_map<std::string, uint32_t> getAgentLineCounts(const google::protobuf::RepeatedPtrField<lyric::Line>& lines);

	/**
	 * Number of lines the given agent performs.
	 */
	uint32_t getAgentLineCount(const google::protobuf::RepeatedPtrField<lyric::Line>& lines, const std::string& id);

	/**
	 * Global and block occurrence index of each line's agent, keyed by the line.
	 */
	std::unordered_map<const lyric::Line*, AgentLineIndex> getAgentLineIndexes(const google::protobuf::RepeatedPtrField<lyric::Line>& lines);

	/**
	 * Agent performing the most lines, null when empty.
	 */
	const lyric::AgentItem* getPrimaryAgent(const lyric::Info& info);
} // namespace music_lyric_model

#endif
