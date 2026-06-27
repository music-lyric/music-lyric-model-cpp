#include "info.h"

#include <stdexcept>

#include "google/protobuf/util/json_util.h"

#include "version.h"

namespace music_lyric_model {
	lyric::Info makeInfo() {
		lyric::Info info;
		info.set_version(SCHEMA_VERSION);
		return info;
	}

	std::vector<uint8_t> encodeInfoWithBinary(const lyric::Info &info) {
		std::vector<uint8_t> out(info.ByteSizeLong());
		if (!info.SerializeToArray(out.data(), static_cast<int>(out.size()))) {
			throw std::runtime_error("failed to encode Info to binary");
		}
		return out;
	}

	lyric::Info decodeInfoWithBinary(const std::vector<uint8_t> &data) {
		lyric::Info info;
		if (!info.ParseFromArray(data.data(), static_cast<int>(data.size()))) {
			throw std::runtime_error("failed to decode Info from binary");
		}
		return info;
	}

	std::string encodeInfoWithJson(const lyric::Info &info) {
		std::string out;
		const auto status = google::protobuf::util::MessageToJsonString(info, &out);
		if (!status.ok()) {
			throw std::runtime_error("failed to encode Info to JSON: " + std::string(status.message()));
		}
		return out;
	}

	lyric::Info decodeInfoWithJson(const std::string &json) {
		lyric::Info info;
		const auto status = google::protobuf::util::JsonStringToMessage(json, &info);
		if (!status.ok()) {
			throw std::runtime_error("failed to decode Info from JSON: " + std::string(status.message()));
		}
		return info;
	}
}
