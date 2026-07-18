#include "parsed/info.h"

#include <stdexcept>

#include "bridge/parsed/info.gen.h"
#include "google/protobuf/util/json_util.h"
#include "parsed/info.pb.h"

namespace music_lyric_model::parsed {
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
} // namespace music_lyric_model::parsed
