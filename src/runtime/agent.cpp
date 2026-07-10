#include "runtime/agent.h"

namespace music_lyric_model::runtime {
	namespace {
		/**
		 * Referenced agent id of a line, null when the line performs no agent.
		 */
		const std::string* lineAgentId(const lyric::runtime::Line& line) {
			if (line.body_case() != lyric::runtime::Line::kNormal) {
				return nullptr;
			}
			const lyric::runtime::LineNormal& normal = line.normal();
			if (!normal.has_content() || !normal.content().has_agent()) {
				return nullptr;
			}
			return &normal.content().agent().id();
		}
	} // namespace

	lyric::runtime::AgentItem makeAgentItem(const lyric::runtime::AgentItem& item) {
		return item;
	}

	lyric::runtime::LineAgent makeLineAgent(const lyric::runtime::LineAgent& agent) {
		return agent;
	}

	const lyric::runtime::AgentItem* getAgentById(const google::protobuf::RepeatedPtrField<lyric::runtime::AgentItem>& agents, const std::string& id) {
		for (const auto& item : agents) {
			if (item.id() == id) {
				return &item;
			}
		}
		return nullptr;
	}

	const lyric::runtime::AgentItem* resolveLineAgent(const lyric::runtime::Line& line, const google::protobuf::RepeatedPtrField<lyric::runtime::AgentItem>& agents) {
		const std::string* id = lineAgentId(line);
		return id ? getAgentById(agents, *id) : nullptr;
	}

	std::unordered_map<std::string, uint32_t> getAgentLineCounts(const google::protobuf::RepeatedPtrField<lyric::runtime::Line>& lines) {
		std::unordered_map<std::string, uint32_t> result;
		for (const auto& line : lines) {
			const std::string* id = lineAgentId(line);
			if (!id) {
				continue;
			}
			result[*id] += 1;
		}
		return result;
	}

	uint32_t getAgentLineCount(const google::protobuf::RepeatedPtrField<lyric::runtime::Line>& lines, const std::string& id) {
		const std::unordered_map<std::string, uint32_t> counts = getAgentLineCounts(lines);
		const auto                                      it     = counts.find(id);
		return it != counts.end() ? it->second : 0;
	}

	std::unordered_map<const lyric::runtime::Line*, AgentLineIndex> getAgentLineIndexes(const google::protobuf::RepeatedPtrField<lyric::runtime::Line>& lines) {
		std::unordered_map<const lyric::runtime::Line*, AgentLineIndex> result;
		std::unordered_map<std::string, uint32_t>                       globals;

		const std::string* prevId     = nullptr;
		uint32_t           blockIndex = 0;

		for (const auto& line : lines) {
			const std::string* id = lineAgentId(line);
			if (!id) {
				continue;
			}

			const uint32_t globalIndex = globals.count(*id) ? globals[*id] : 0;
			globals[*id]               = globalIndex + 1;

			if (!prevId || *id != *prevId) {
				blockIndex = 0;
				prevId     = id;
			}

			result[&line] = AgentLineIndex{globalIndex, blockIndex++};
		}

		return result;
	}

	const lyric::runtime::AgentItem* getPrimaryAgent(const lyric::runtime::Info& info) {
		const std::unordered_map<std::string, uint32_t> counts = getAgentLineCounts(info.lines());

		const lyric::runtime::AgentItem* result = nullptr;
		int64_t                          max    = -1;
		for (const auto& agent : info.agents()) {
			const auto    it    = counts.find(agent.id());
			const int64_t count = it != counts.end() ? it->second : 0;
			if (count > max) {
				max    = count;
				result = &agent;
			}
		}
		return result;
	}

	std::vector<const lyric::runtime::AgentItem*> getAgentsByType(const google::protobuf::RepeatedPtrField<lyric::runtime::AgentItem>& agents, lyric::common::AgentType type) {
		std::vector<const lyric::runtime::AgentItem*> result;
		for (const auto& item : agents) {
			if (item.type() == type) {
				result.push_back(&item);
			}
		}
		return result;
	}

	bool hasAgent(const google::protobuf::RepeatedPtrField<lyric::runtime::AgentItem>& agents, lyric::common::AgentType type) {
		for (const auto& item : agents) {
			if (item.type() == type) {
				return true;
			}
		}
		return false;
	}
} // namespace music_lyric_model::runtime
