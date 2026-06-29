#ifndef MUSIC_LYRIC_AGENT_AGENT_H_
#define MUSIC_LYRIC_AGENT_AGENT_H_

#include <string>

#include "agent/agent.pb.h"
#include "line/content.pb.h"

namespace music_lyric_model {
	using namespace lyric;

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
	const lyric::AgentItem *getAgentById(const google::protobuf::RepeatedPtrField<lyric::AgentItem> &agents, const std::string &id);

	/**
	 * Agent performing a line, resolved from its reference.
	 */
	const lyric::AgentItem *getLineAgent(const lyric::Line &line, const google::protobuf::RepeatedPtrField<lyric::AgentItem> &agents);

	/**
	 * Agent performing the most lines, null when empty.
	 */
	const lyric::AgentItem *getPrimaryAgent(const google::protobuf::RepeatedPtrField<lyric::AgentItem> &agents);
}

#endif
