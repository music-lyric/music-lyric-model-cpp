#include "storage/agent.h"

namespace music_lyric_model::storage {
	lyric::storage::AgentItem makeAgentItem(const lyric::storage::AgentItem& item) {
		return item;
	}

	const lyric::storage::AgentItem* getAgentById(const google::protobuf::RepeatedPtrField<lyric::storage::AgentItem>& agents, const std::string& id) {
		for (const auto& item : agents) {
			if (item.id() == id) {
				return &item;
			}
		}
		return nullptr;
	}

	std::vector<const lyric::storage::AgentItem*> resolveLineAgents(const lyric::storage::Line& line, const google::protobuf::RepeatedPtrField<lyric::storage::AgentItem>& agents) {
		std::vector<const lyric::storage::AgentItem*> result;
		if (!line.has_content()) {
			return result;
		}
		for (const auto& id : line.content().agents()) {
			const lyric::storage::AgentItem* agent = getAgentById(agents, id);
			if (agent) {
				result.push_back(agent);
			}
		}
		return result;
	}

	std::unordered_map<std::string, uint32_t> getAgentLineCounts(const google::protobuf::RepeatedPtrField<lyric::storage::Line>& lines) {
		std::unordered_map<std::string, uint32_t> result;
		for (const auto& line : lines) {
			if (!line.has_content()) {
				continue;
			}
			for (const auto& id : line.content().agents()) {
				result[id] += 1;
			}
		}
		return result;
	}

	uint32_t getAgentLineCount(const google::protobuf::RepeatedPtrField<lyric::storage::Line>& lines, const std::string& id) {
		const std::unordered_map<std::string, uint32_t> counts = getAgentLineCounts(lines);
		const auto                                      it     = counts.find(id);
		return it != counts.end() ? it->second : 0;
	}

	const lyric::storage::AgentItem* getPrimaryAgent(const lyric::storage::Lyric& model) {
		const std::unordered_map<std::string, uint32_t> counts = getAgentLineCounts(model.lines());

		const lyric::storage::AgentItem* result = nullptr;
		int64_t                          max    = -1;
		for (const auto& agent : model.agents()) {
			const auto    it    = counts.find(agent.id());
			const int64_t count = it != counts.end() ? it->second : 0;
			if (count > max) {
				max    = count;
				result = &agent;
			}
		}
		return result;
	}

	std::vector<const lyric::storage::AgentItem*> getAgentsByType(const google::protobuf::RepeatedPtrField<lyric::storage::AgentItem>& agents, lyric::common::AgentType type) {
		std::vector<const lyric::storage::AgentItem*> result;
		for (const auto& item : agents) {
			if (item.type() == type) {
				result.push_back(&item);
			}
		}
		return result;
	}

	bool hasAgent(const google::protobuf::RepeatedPtrField<lyric::storage::AgentItem>& agents, lyric::common::AgentType type) {
		for (const auto& item : agents) {
			if (item.type() == type) {
				return true;
			}
		}
		return false;
	}
} // namespace music_lyric_model::storage
