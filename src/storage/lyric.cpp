#include "storage/lyric.h"

#include <algorithm>
#include <stdexcept>

#include "google/protobuf/util/json_util.h"
#include "storage/line.h"
#include "version.h"

namespace music_lyric_model::storage {
	lyric::storage::Lyric makeStorageLyric(const lyric::storage::Lyric& init) {
		lyric::storage::Lyric lyric = init;
		lyric.set_version(SCHEMA_VERSION);
		return lyric;
	}

	std::vector<uint8_t> encodeStorageLyric(const lyric::storage::Lyric& lyric) {
		std::vector<uint8_t> out(lyric.ByteSizeLong());
		if (!lyric.SerializeToArray(out.data(), static_cast<int>(out.size()))) {
			throw std::runtime_error("failed to encode Lyric to binary");
		}
		return out;
	}

	lyric::storage::Lyric decodeStorageLyric(const std::vector<uint8_t>& data) {
		lyric::storage::Lyric lyric;
		if (!lyric.ParseFromArray(data.data(), static_cast<int>(data.size()))) {
			throw std::runtime_error("failed to decode Lyric from binary");
		}
		return lyric;
	}

	std::string storageLyricToJson(const lyric::storage::Lyric& lyric) {
		std::string out;
		const auto  status = google::protobuf::util::MessageToJsonString(lyric, &out);
		if (!status.ok()) {
			throw std::runtime_error("failed to encode Lyric to JSON: " + std::string(status.message()));
		}
		return out;
	}

	lyric::storage::Lyric storageLyricFromJson(const std::string& json) {
		lyric::storage::Lyric lyric;
		const auto            status = google::protobuf::util::JsonStringToMessage(json, &lyric);
		if (!status.ok()) {
			throw std::runtime_error("failed to decode Lyric from JSON: " + std::string(status.message()));
		}
		return lyric;
	}

	void sortStorageLinesByTime(lyric::storage::Lyric& lyric) {
		std::stable_sort(lyric.mutable_lines()->begin(), lyric.mutable_lines()->end(), [](const lyric::storage::Line& a, const lyric::storage::Line& b) {
			const lyric::common::Time* ta = getStorageLineTime(a);
			const lyric::common::Time* tb = getStorageLineTime(b);
			return (ta ? ta->start() : 0) < (tb ? tb->start() : 0);
		});

		for (auto& line : *lyric.mutable_lines()) {
			std::stable_sort(line.mutable_backgrounds()->begin(), line.mutable_backgrounds()->end(), [](const lyric::storage::LineBackground& a, const lyric::storage::LineBackground& b) {
				const uint32_t sa = a.has_time() ? a.time().start() : 0;
				const uint32_t sb = b.has_time() ? b.time().start() : 0;
				return sa < sb;
			});
		}
	}
} // namespace music_lyric_model::storage
