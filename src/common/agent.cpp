#include "common/agent.h"

namespace music_lyric_model::common {
	lyric::common::AgentItem makeAgentItem(const lyric::common::AgentItem& item) {
		return item;
	}

	const lyric::common::AgentItem* getAgentById(const google::protobuf::RepeatedPtrField<lyric::common::AgentItem>& agents, const std::string& id) {
		for (const auto& item : agents) {
			if (item.id() == id) {
				return &item;
			}
		}
		return nullptr;
	}

	std::vector<const lyric::common::AgentItem*> getAgentsByType(const google::protobuf::RepeatedPtrField<lyric::common::AgentItem>& agents, lyric::common::AgentType type) {
		std::vector<const lyric::common::AgentItem*> result;
		for (const auto& item : agents) {
			if (item.type() == type) {
				result.push_back(&item);
			}
		}
		return result;
	}

	bool hasAgent(const google::protobuf::RepeatedPtrField<lyric::common::AgentItem>& agents, lyric::common::AgentType type) {
		for (const auto& item : agents) {
			if (item.type() == type) {
				return true;
			}
		}
		return false;
	}
} // namespace music_lyric_model::common
