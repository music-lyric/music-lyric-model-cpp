#include "parsed/info.h"

#include <algorithm>
#include <stdexcept>

#include "bridge/parsed/info.gen.h"
#include "google/protobuf/util/json_util.h"
#include "parsed/info.pb.h"
#include "parsed/line.h"
#include "version.h"

namespace music_lyric_model::parsed {
	Info makeParsedInfo(Info init) {
		init.version = SCHEMA_VERSION;
		return init;
	}

	bool isParsedInfoValid(const Info& info) {
		return info.type == InfoType::Valid;
	}

	bool isParsedInfoInvalid(const Info& info) {
		return info.type == InfoType::Invalid;
	}

	std::vector<uint8_t> encodeParsedInfo(const Info& info) {
		const lyric::parsed::Info pb = internal::toPb(info);
		std::vector<uint8_t>      out(pb.ByteSizeLong());
		if (!pb.SerializeToArray(out.data(), static_cast<int>(out.size()))) {
			throw std::runtime_error("failed to encode Info to binary");
		}
		return out;
	}

	Info decodeParsedInfo(const std::vector<uint8_t>& data) {
		lyric::parsed::Info pb;
		if (!pb.ParseFromArray(data.data(), static_cast<int>(data.size()))) {
			throw std::runtime_error("failed to decode Info from binary");
		}
		return internal::fromPb(pb);
	}

	std::string parsedInfoToJson(const Info& info) {
		const lyric::parsed::Info pb = internal::toPb(info);
		std::string               out;
		const auto                status = google::protobuf::util::MessageToJsonString(pb, &out);
		if (!status.ok()) {
			throw std::runtime_error("failed to encode Info to JSON: " + std::string(status.message()));
		}
		return out;
	}

	Info parsedInfoFromJson(const std::string& json) {
		lyric::parsed::Info pb;
		const auto          status = google::protobuf::util::JsonStringToMessage(json, &pb);
		if (!status.ok()) {
			throw std::runtime_error("failed to decode Info from JSON: " + std::string(status.message()));
		}
		return internal::fromPb(pb);
	}

	void sortParsedLinesByTime(Info& info) {
		std::stable_sort(info.lines.begin(), info.lines.end(), [](const Line& a, const Line& b) {
			const common::Time* ta = getParsedLineTime(a);
			const common::Time* tb = getParsedLineTime(b);
			return (ta ? ta->start : 0) < (tb ? tb->start : 0);
		});

		for (auto& line : info.lines) {
			LineNormal* normal = std::get_if<LineNormal>(&line);
			if (!normal) {
				continue;
			}
			std::stable_sort(normal->backgrounds.begin(), normal->backgrounds.end(), [](const LineBackground& a, const LineBackground& b) {
				const uint32_t sa = a.time.has_value() ? a.time->start : 0;
				const uint32_t sb = b.time.has_value() ? b.time->start : 0;
				return sa < sb;
			});
		}
	}
} // namespace music_lyric_model::parsed
