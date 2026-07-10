#include "runtime/info.h"

#include <algorithm>
#include <stdexcept>
#include <string>

#include "google/protobuf/util/json_util.h"
#include "runtime/line.h"
#include "version.h"

namespace music_lyric_model {
	lyric::runtime::Info makeInfo(const lyric::runtime::Info& init) {
		lyric::runtime::Info info = init;
		info.set_version(SCHEMA_VERSION);
		return info;
	}

	std::vector<uint8_t> encodeInfo(const lyric::runtime::Info& info) {
		std::vector<uint8_t> out(info.ByteSizeLong());
		if (!info.SerializeToArray(out.data(), static_cast<int>(out.size()))) {
			throw std::runtime_error("failed to encode Info to binary");
		}
		return out;
	}

	lyric::runtime::Info decodeInfo(const std::vector<uint8_t>& data) {
		lyric::runtime::Info info;
		if (!info.ParseFromArray(data.data(), static_cast<int>(data.size()))) {
			throw std::runtime_error("failed to decode Info from binary");
		}
		return info;
	}

	std::string infoToJson(const lyric::runtime::Info& info) {
		std::string out;
		const auto  status = google::protobuf::util::MessageToJsonString(info, &out);
		if (!status.ok()) {
			throw std::runtime_error("failed to encode Info to JSON: " + std::string(status.message()));
		}
		return out;
	}

	lyric::runtime::Info infoFromJson(const std::string& json) {
		lyric::runtime::Info info;
		const auto           status = google::protobuf::util::JsonStringToMessage(json, &info);
		if (!status.ok()) {
			throw std::runtime_error("failed to decode Info from JSON: " + std::string(status.message()));
		}
		return info;
	}

	void sortLinesByTime(lyric::runtime::Info& info) {
		std::stable_sort(info.mutable_lines()->begin(), info.mutable_lines()->end(), [](const lyric::runtime::Line& a, const lyric::runtime::Line& b) {
			const lyric::common::Time* ta = getLineTime(a);
			const lyric::common::Time* tb = getLineTime(b);
			return (ta ? ta->start() : 0) < (tb ? tb->start() : 0);
		});

		for (auto& line : *info.mutable_lines()) {
			if (line.body_case() != lyric::runtime::Line::kNormal) {
				continue;
			}
			auto* backgrounds = line.mutable_normal()->mutable_backgrounds();
			std::stable_sort(backgrounds->begin(), backgrounds->end(), [](const lyric::runtime::LineBackground& a, const lyric::runtime::LineBackground& b) {
				const uint32_t sa = a.has_time() ? a.time().start() : 0;
				const uint32_t sb = b.has_time() ? b.time().start() : 0;
				return sa < sb;
			});
		}
	}
} // namespace music_lyric_model
