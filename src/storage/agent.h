#ifndef MUSIC_LYRIC_STORAGE_AGENT_H_
#define MUSIC_LYRIC_STORAGE_AGENT_H_

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/agent.pb.h"
#include "storage/agent/agent.pb.h"
#include "storage/line/content.pb.h"
#include "storage/lyric.pb.h"

namespace music_lyric_model::storage {
	/**
	 * Creates an AgentItem.
	 */
	lyric::storage::AgentItem makeAgentItem(const lyric::storage::AgentItem& item = {});

	/**
	 * Agent with the given id, null when absent.
	 */
	const lyric::storage::AgentItem* getAgentById(const google::protobuf::RepeatedPtrField<lyric::storage::AgentItem>& agents, const std::string& id);

	/**
	 * Agents performing a line, resolved from its id references.
	 */
	std::vector<const lyric::storage::AgentItem*> resolveLineAgents(const lyric::storage::Line& line, const google::protobuf::RepeatedPtrField<lyric::storage::AgentItem>& agents);

	/**
	 * Number of lines each agent performs, keyed by agent id.
	 */
	std::unordered_map<std::string, uint32_t> getAgentLineCounts(const google::protobuf::RepeatedPtrField<lyric::storage::Line>& lines);

	/**
	 * Number of lines the given agent performs.
	 */
	uint32_t getAgentLineCount(const google::protobuf::RepeatedPtrField<lyric::storage::Line>& lines, const std::string& id);

	/**
	 * Agent performing the most lines, null when empty.
	 */
	const lyric::storage::AgentItem* getPrimaryAgent(const lyric::storage::Lyric& model);

	/**
	 * All agents of a type.
	 */
	std::vector<const lyric::storage::AgentItem*> getAgentsByType(const google::protobuf::RepeatedPtrField<lyric::storage::AgentItem>& agents, lyric::common::AgentType type);

	/**
	 * Whether any agent of a type exists.
	 */
	bool hasAgent(const google::protobuf::RepeatedPtrField<lyric::storage::AgentItem>& agents, lyric::common::AgentType type);
} // namespace music_lyric_model::storage

#endif
