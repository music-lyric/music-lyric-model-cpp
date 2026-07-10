#include "storage/lyric.h"

#include <algorithm>
#include <stdexcept>
#include <string>

#include "google/protobuf/util/json_util.h"
#include "storage/line.h"
#include "version.h"

namespace music_lyric_model::storage {
	lyric::storage::Lyric makeLyric(const lyric::storage::Lyric& init) {
		lyric::storage::Lyric result = init;
		result.set_version(SCHEMA_VERSION);
		return result;
	}

	std::vector<uint8_t> encodeLyric(const lyric::storage::Lyric& model) {
		std::vector<uint8_t> out(model.ByteSizeLong());
		if (!model.SerializeToArray(out.data(), static_cast<int>(out.size()))) {
			throw std::runtime_error("failed to encode Lyric to binary");
		}
		return out;
	}

	lyric::storage::Lyric decodeLyric(const std::vector<uint8_t>& data) {
		lyric::storage::Lyric result;
		if (!result.ParseFromArray(data.data(), static_cast<int>(data.size()))) {
			throw std::runtime_error("failed to decode Lyric from binary");
		}
		return result;
	}

	std::string lyricToJson(const lyric::storage::Lyric& model) {
		std::string out;
		const auto  status = google::protobuf::util::MessageToJsonString(model, &out);
		if (!status.ok()) {
			throw std::runtime_error("failed to encode Lyric to JSON: " + std::string(status.message()));
		}
		return out;
	}

	lyric::storage::Lyric lyricFromJson(const std::string& json) {
		lyric::storage::Lyric result;
		const auto            status = google::protobuf::util::JsonStringToMessage(json, &result);
		if (!status.ok()) {
			throw std::runtime_error("failed to decode Lyric from JSON: " + std::string(status.message()));
		}
		return result;
	}

	void sortLinesByTime(lyric::storage::Lyric& model) {
		std::stable_sort(model.mutable_lines()->begin(), model.mutable_lines()->end(), [](const lyric::storage::Line& a, const lyric::storage::Line& b) {
			const lyric::common::Time* ta = getLineTime(a);
			const lyric::common::Time* tb = getLineTime(b);
			return (ta ? ta->start() : 0) < (tb ? tb->start() : 0);
		});

		for (auto& line : *model.mutable_lines()) {
			auto* backgrounds = line.mutable_backgrounds();
			std::stable_sort(backgrounds->begin(), backgrounds->end(), [](const lyric::storage::LineContent& a, const lyric::storage::LineContent& b) {
				const uint32_t sa = a.has_time() ? a.time().start() : 0;
				const uint32_t sb = b.has_time() ? b.time().start() : 0;
				return sa < sb;
			});
		}
	}
} // namespace music_lyric_model::storage
