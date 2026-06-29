#include "agent/agent.h"

namespace music_lyric_model {
	lyric::AgentItem makeAgentItem() {
		return {};
	}

	lyric::LineAgent makeLineAgent() {
		return {};
	}

	const lyric::AgentItem* getAgentById(const google::protobuf::RepeatedPtrField<lyric::AgentItem>& agents, const std::string& id) {
		for (const auto& item : agents) {
			if (item.id() == id) {
				return &item;
			}
		}
		return nullptr;
	}

	const lyric::AgentItem* getLineAgent(const lyric::Line& line, const google::protobuf::RepeatedPtrField<lyric::AgentItem>& agents) {
		if (line.body_case() != lyric::Line::kNormal) {
			return nullptr;
		}
		const lyric::LineNormal& normal = line.normal();
		if (!normal.has_agent()) {
			return nullptr;
		}
		return getAgentById(agents, normal.agent().id());
	}

	const lyric::AgentItem* getPrimaryAgent(const google::protobuf::RepeatedPtrField<lyric::AgentItem>& agents) {
		const lyric::AgentItem* result = nullptr;
		for (const auto& item : agents) {
			if (!result || item.count() > result->count()) {
				result = &item;
			}
		}
		return result;
	}
} // namespace music_lyric_model
