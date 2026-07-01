#include "info.h"

#include <algorithm>
#include <stdexcept>
#include <string>

#include "google/protobuf/util/json_util.h"
#include "line/content.h"
#include "version.h"

namespace music_lyric_model {
	lyric::Info makeInfo() {
		lyric::Info info;
		info.set_version(SCHEMA_VERSION);
		return info;
	}

	std::vector<uint8_t> encodeInfo(const lyric::Info& info) {
		std::vector<uint8_t> out(info.ByteSizeLong());
		if (!info.SerializeToArray(out.data(), static_cast<int>(out.size()))) {
			throw std::runtime_error("failed to encode Info to binary");
		}
		return out;
	}

	lyric::Info decodeInfo(const std::vector<uint8_t>& data) {
		lyric::Info info;
		if (!info.ParseFromArray(data.data(), static_cast<int>(data.size()))) {
			throw std::runtime_error("failed to decode Info from binary");
		}
		return info;
	}

	std::string infoToJson(const lyric::Info& info) {
		std::string out;
		const auto  status = google::protobuf::util::MessageToJsonString(info, &out);
		if (!status.ok()) {
			throw std::runtime_error("failed to encode Info to JSON: " + std::string(status.message()));
		}
		return out;
	}

	lyric::Info infoFromJson(const std::string& json) {
		lyric::Info info;
		const auto  status = google::protobuf::util::JsonStringToMessage(json, &info);
		if (!status.ok()) {
			throw std::runtime_error("failed to decode Info from JSON: " + std::string(status.message()));
		}
		return info;
	}

	void sortLinesByTime(lyric::Info& info) {
		std::stable_sort(info.mutable_lines()->begin(), info.mutable_lines()->end(), [](const lyric::Line& a, const lyric::Line& b) {
			const lyric::Time* ta = getLineTime(a);
			const lyric::Time* tb = getLineTime(b);
			return (ta ? ta->start() : 0) < (tb ? tb->start() : 0);
		});

		for (auto& line : *info.mutable_lines()) {
			if (line.body_case() != lyric::Line::kNormal) {
				continue;
			}
			auto* backgrounds = line.mutable_normal()->mutable_backgrounds();
			std::stable_sort(backgrounds->begin(), backgrounds->end(), [](const lyric::LineBackground& a, const lyric::LineBackground& b) {
				const uint32_t sa = a.has_time() ? a.time().start() : 0;
				const uint32_t sb = b.has_time() ? b.time().start() : 0;
				return sa < sb;
			});
		}
	}
} // namespace music_lyric_model
