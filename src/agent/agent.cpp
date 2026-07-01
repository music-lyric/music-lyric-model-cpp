#include "agent/agent.h"

namespace music_lyric_model {
	namespace {
		/**
		 * Referenced agent id of a line, null when the line performs no agent.
		 */
		const std::string* lineAgentId(const lyric::Line& line) {
			if (line.body_case() != lyric::Line::kNormal) {
				return nullptr;
			}
			const lyric::LineNormal& normal = line.normal();
			if (!normal.has_content() || !normal.content().has_agent()) {
				return nullptr;
			}
			return &normal.content().agent().id();
		}
	} // namespace

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
		const std::string* id = lineAgentId(line);
		return id ? getAgentById(agents, *id) : nullptr;
	}

	std::unordered_map<std::string, uint32_t> getAgentLineCounts(const google::protobuf::RepeatedPtrField<lyric::Line>& lines) {
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

	uint32_t getAgentLineCount(const google::protobuf::RepeatedPtrField<lyric::Line>& lines, const std::string& id) {
		const std::unordered_map<std::string, uint32_t> counts = getAgentLineCounts(lines);
		const auto                                      it     = counts.find(id);
		return it != counts.end() ? it->second : 0;
	}

	std::unordered_map<const lyric::Line*, AgentLineIndex> getAgentLineIndexes(const google::protobuf::RepeatedPtrField<lyric::Line>& lines) {
		std::unordered_map<const lyric::Line*, AgentLineIndex> result;
		std::unordered_map<std::string, uint32_t>              globals;

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

	const lyric::AgentItem* getPrimaryAgent(const lyric::Info& info) {
		const std::unordered_map<std::string, uint32_t> counts = getAgentLineCounts(info.lines());

		const lyric::AgentItem* result = nullptr;
		int64_t                 max    = -1;
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
} // namespace music_lyric_model
