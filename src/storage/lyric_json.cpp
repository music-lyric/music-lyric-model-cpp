#include "storage/lyric.h"

#include <stdexcept>

#include "bridge/storage/lyric.gen.h"
#include "google/protobuf/util/json_util.h"
#include "storage/lyric.pb.h"

namespace music_lyric_model::storage {
	std::string storageLyricToJson(const Lyric& lyric) {
		const lyric::storage::Lyric pb = internal::toPb(lyric);
		std::string                 out;
		const auto                  status = google::protobuf::util::MessageToJsonString(pb, &out);
		if (!status.ok()) {
			throw std::runtime_error("failed to encode Lyric to JSON: " + std::string(status.message()));
		}
		return out;
	}

	Lyric storageLyricFromJson(const std::string& json) {
		lyric::storage::Lyric pb;
		const auto            status = google::protobuf::util::JsonStringToMessage(json, &pb);
		if (!status.ok()) {
			throw std::runtime_error("failed to decode Lyric from JSON: " + std::string(status.message()));
		}
		return internal::fromPb(pb);
	}
} // namespace music_lyric_model::storage
