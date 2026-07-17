#ifndef MUSIC_LYRIC_MODEL_COMMON_AGENT_H_
#define MUSIC_LYRIC_MODEL_COMMON_AGENT_H_

#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "common/agent.pb.h"

namespace music_lyric_model::common {
	/**
	 * Creates an AgentItem.
	 */
	lyric::common::AgentItem makeAgentItem(const lyric::common::AgentItem& item = {});

	/**
	 * Agent with the given id, null when absent.
	 */
	const lyric::common::AgentItem* getAgentById(const google::protobuf::RepeatedPtrField<lyric::common::AgentItem>& agents, const std::string& id);

	/**
	 * Agents resolved from id references, in reference order.
	 * Missing ids are skipped.
	 */
	template <typename Ids>
	std::vector<const lyric::common::AgentItem*> resolveAgents(const google::protobuf::RepeatedPtrField<lyric::common::AgentItem>& agents, const Ids& ids) {
		std::vector<const lyric::common::AgentItem*> result;
		for (const auto& id : ids) {
			if (const lyric::common::AgentItem* item = getAgentById(agents, id)) {
				result.push_back(item);
			}
		}
		return result;
	}

	/**
	 * Agents resolved from a line's agent id references, in reference order.
	 * Missing ids are skipped.
	 */
	template <typename LineLike>
	std::vector<const lyric::common::AgentItem*> resolveLineAgents(const google::protobuf::RepeatedPtrField<lyric::common::AgentItem>& agents, const LineLike& line) {
		return resolveAgents(agents, line.agents());
	}

	/**
	 * How many lines reference each agent id.
	 * Each id is counted at most once per line.
	 */
	template <typename Lines>
	std::unordered_map<std::string, uint32_t> getAgentLineCounts(const Lines& lines) {
		std::unordered_map<std::string, uint32_t> counts;
		for (const auto& line : lines) {
			std::unordered_set<std::string> seen;
			for (const auto& id : line.agents()) {
				if (!seen.insert(id).second) {
					continue;
				}
				counts[id] += 1;
			}
		}
		return counts;
	}

	/**
	 * Agent referenced by the most lines.
	 * Ties keep the earlier entry in the agents list.
	 * Returns null when no listed agent is referenced.
	 */
	template <typename Lines>
	const lyric::common::AgentItem* getPrimaryAgent(const google::protobuf::RepeatedPtrField<lyric::common::AgentItem>& agents, const Lines& lines) {
		const std::unordered_map<std::string, uint32_t> counts = getAgentLineCounts(lines);
		const lyric::common::AgentItem*                 result = nullptr;
		uint32_t                                        best   = 0;
		for (const auto& agent : agents) {
			const auto     it    = counts.find(agent.id());
			const uint32_t count = it != counts.end() ? it->second : 0;
			if (count > best) {
				best   = count;
				result = &agent;
			}
		}
		return result;
	}

	/**
	 * All agents of a type.
	 */
	std::vector<const lyric::common::AgentItem*> getAgentsByType(const google::protobuf::RepeatedPtrField<lyric::common::AgentItem>& agents, lyric::common::AgentType type);

	/**
	 * Whether any agent of a type exists.
	 */
	bool hasAgent(const google::protobuf::RepeatedPtrField<lyric::common::AgentItem>& agents, lyric::common::AgentType type);
} // namespace music_lyric_model::common

#endif
