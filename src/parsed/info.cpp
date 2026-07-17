#include "parsed/info.h"

#include <algorithm>
#include <stdexcept>

#include "google/protobuf/util/json_util.h"
#include "parsed/line.h"
#include "version.h"

namespace music_lyric_model::parsed {
	lyric::parsed::Info makeParsedInfo(const lyric::parsed::Info& init) {
		lyric::parsed::Info info = init;
		info.set_version(SCHEMA_VERSION);
		return info;
	}

	bool isParsedInfoValid(const lyric::parsed::Info& info) {
		return info.type() == lyric::parsed::INFO_TYPE_VALID;
	}

	bool isParsedInfoInvalid(const lyric::parsed::Info& info) {
		return info.type() == lyric::parsed::INFO_TYPE_INVALID;
	}

	std::vector<uint8_t> encodeParsedInfo(const lyric::parsed::Info& info) {
		std::vector<uint8_t> out(info.ByteSizeLong());
		if (!info.SerializeToArray(out.data(), static_cast<int>(out.size()))) {
			throw std::runtime_error("failed to encode Info to binary");
		}
		return out;
	}

	lyric::parsed::Info decodeParsedInfo(const std::vector<uint8_t>& data) {
		lyric::parsed::Info info;
		if (!info.ParseFromArray(data.data(), static_cast<int>(data.size()))) {
			throw std::runtime_error("failed to decode Info from binary");
		}
		return info;
	}

	std::string parsedInfoToJson(const lyric::parsed::Info& info) {
		std::string out;
		const auto  status = google::protobuf::util::MessageToJsonString(info, &out);
		if (!status.ok()) {
			throw std::runtime_error("failed to encode Info to JSON: " + std::string(status.message()));
		}
		return out;
	}

	lyric::parsed::Info parsedInfoFromJson(const std::string& json) {
		lyric::parsed::Info info;
		const auto          status = google::protobuf::util::JsonStringToMessage(json, &info);
		if (!status.ok()) {
			throw std::runtime_error("failed to decode Info from JSON: " + std::string(status.message()));
		}
		return info;
	}

	void sortParsedLinesByTime(lyric::parsed::Info& info) {
		std::stable_sort(info.mutable_lines()->begin(), info.mutable_lines()->end(), [](const lyric::parsed::Line& a, const lyric::parsed::Line& b) {
			const lyric::common::Time* ta = getParsedLineTime(a);
			const lyric::common::Time* tb = getParsedLineTime(b);
			return (ta ? ta->start() : 0) < (tb ? tb->start() : 0);
		});

		for (auto& line : *info.mutable_lines()) {
			if (line.body_case() != lyric::parsed::Line::kNormal) {
				continue;
			}
			auto* backgrounds = line.mutable_normal()->mutable_backgrounds();
			std::stable_sort(backgrounds->begin(), backgrounds->end(), [](const lyric::parsed::LineBackground& a, const lyric::parsed::LineBackground& b) {
				const uint32_t sa = a.has_time() ? a.time().start() : 0;
				const uint32_t sb = b.has_time() ? b.time().start() : 0;
				return sa < sb;
			});
		}
	}
} // namespace music_lyric_model::parsed
