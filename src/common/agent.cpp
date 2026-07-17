#include "common/agent.h"

namespace music_lyric_model::common {
	const AgentItem* getAgentById(const std::vector<AgentItem>& agents, const std::string& id) {
		for (const auto& item : agents) {
			if (item.id == id) {
				return &item;
			}
		}
		return nullptr;
	}

	std::vector<const AgentItem*> getAgentsByType(const std::vector<AgentItem>& agents, AgentType type) {
		std::vector<const AgentItem*> result;
		for (const auto& item : agents) {
			if (item.type == type) {
				result.push_back(&item);
			}
		}
		return result;
	}

	bool hasAgent(const std::vector<AgentItem>& agents, AgentType type) {
		for (const auto& item : agents) {
			if (item.type == type) {
				return true;
			}
		}
		return false;
	}
} // namespace music_lyric_model::common
