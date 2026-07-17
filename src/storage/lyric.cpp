#include "storage/lyric.h"

#include <algorithm>
#include <stdexcept>

#include "google/protobuf/util/json_util.h"
#include "pb.h"
#include "storage/line.h"
#include "storage/line.pb.h"
#include "storage/lyric.pb.h"
#include "version.h"

namespace music_lyric_model::storage {
	namespace {
		/**
		 * Convert a public LineBackground to its protobuf form.
		 */
		lyric::storage::LineBackground toPb(const LineBackground& line) {
			lyric::storage::LineBackground out;
			internal::putMap(line.extra, out.mutable_extra());
			if (line.time.has_value()) {
				*out.mutable_time() = internal::toPb(*line.time);
			}
			for (const auto& agent : line.agents) {
				out.add_agents(agent);
			}
			for (const auto& word : line.words) {
				*out.add_words() = internal::toPb(word);
			}
			if (line.annotation.has_value()) {
				*out.mutable_annotation() = internal::toPb(*line.annotation);
			}
			return out;
		}

		/**
		 * Convert a protobuf LineBackground to the public form.
		 */
		LineBackground fromPb(const lyric::storage::LineBackground& line) {
			LineBackground out;
			out.extra = internal::takeMap(line.extra());
			out.time  = internal::optionalTimeFrom(line.has_time(), line.time());
			out.agents.assign(line.agents().begin(), line.agents().end());
			out.words.reserve(static_cast<size_t>(line.words_size()));
			for (const auto& word : line.words()) {
				out.words.push_back(internal::fromPb(word));
			}
			if (line.has_annotation()) {
				out.annotation = internal::fromPb(line.annotation());
			}
			return out;
		}

		/**
		 * Convert a public Line to its protobuf form.
		 */
		lyric::storage::Line toPb(const Line& line) {
			lyric::storage::Line out;
			internal::putMap(line.extra, out.mutable_extra());
			if (line.time.has_value()) {
				*out.mutable_time() = internal::toPb(*line.time);
			}
			if (line.part.has_value()) {
				*out.mutable_part() = internal::toPb(*line.part);
			}
			for (const auto& agent : line.agents) {
				out.add_agents(agent);
			}
			for (const auto& word : line.words) {
				*out.add_words() = internal::toPb(word);
			}
			if (line.annotation.has_value()) {
				*out.mutable_annotation() = internal::toPb(*line.annotation);
			}
			for (const auto& background : line.backgrounds) {
				*out.add_backgrounds() = toPb(background);
			}
			return out;
		}

		/**
		 * Convert a protobuf Line to the public form.
		 */
		Line fromPb(const lyric::storage::Line& line) {
			Line out;
			out.extra = internal::takeMap(line.extra());
			out.time  = internal::optionalTimeFrom(line.has_time(), line.time());
			if (line.has_part()) {
				out.part = internal::fromPb(line.part());
			}
			out.agents.assign(line.agents().begin(), line.agents().end());
			out.words.reserve(static_cast<size_t>(line.words_size()));
			for (const auto& word : line.words()) {
				out.words.push_back(internal::fromPb(word));
			}
			if (line.has_annotation()) {
				out.annotation = internal::fromPb(line.annotation());
			}
			out.backgrounds.reserve(static_cast<size_t>(line.backgrounds_size()));
			for (const auto& background : line.backgrounds()) {
				out.backgrounds.push_back(fromPb(background));
			}
			return out;
		}

		/**
		 * Convert a public Lyric to its protobuf form.
		 */
		lyric::storage::Lyric toPb(const Lyric& lyric) {
			lyric::storage::Lyric out;
			out.set_version(lyric.version);
			out.set_timing(internal::toPb(lyric.timing));
			internal::putMap(lyric.extra, out.mutable_extra());
			*out.mutable_meta() = internal::toPb(lyric.meta);
			for (const auto& item : lyric.agents) {
				*out.add_agents() = internal::toPb(item);
			}
			for (const auto& item : lyric.lines) {
				*out.add_lines() = toPb(item);
			}
			return out;
		}

		/**
		 * Convert a protobuf Lyric to the public form.
		 */
		Lyric fromPb(const lyric::storage::Lyric& lyric) {
			Lyric out;
			out.version = lyric.version();
			out.timing  = internal::fromPb(lyric.timing());
			out.extra   = internal::takeMap(lyric.extra());
			if (lyric.has_meta()) {
				out.meta = internal::fromPb(lyric.meta());
			}
			out.agents.reserve(static_cast<size_t>(lyric.agents_size()));
			for (const auto& item : lyric.agents()) {
				out.agents.push_back(internal::fromPb(item));
			}
			out.lines.reserve(static_cast<size_t>(lyric.lines_size()));
			for (const auto& item : lyric.lines()) {
				out.lines.push_back(fromPb(item));
			}
			return out;
		}
	} // namespace

	Lyric makeStorageLyric(Lyric init) {
		init.version = SCHEMA_VERSION;
		return init;
	}

	std::vector<uint8_t> encodeStorageLyric(const Lyric& lyric) {
		const lyric::storage::Lyric pb = toPb(lyric);
		std::vector<uint8_t>        out(pb.ByteSizeLong());
		if (!pb.SerializeToArray(out.data(), static_cast<int>(out.size()))) {
			throw std::runtime_error("failed to encode Lyric to binary");
		}
		return out;
	}

	Lyric decodeStorageLyric(const std::vector<uint8_t>& data) {
		lyric::storage::Lyric pb;
		if (!pb.ParseFromArray(data.data(), static_cast<int>(data.size()))) {
			throw std::runtime_error("failed to decode Lyric from binary");
		}
		return fromPb(pb);
	}

	std::string storageLyricToJson(const Lyric& lyric) {
		const lyric::storage::Lyric pb = toPb(lyric);
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
		return fromPb(pb);
	}

	void sortStorageLinesByTime(Lyric& lyric) {
		std::stable_sort(lyric.lines.begin(), lyric.lines.end(), [](const Line& a, const Line& b) {
			const common::Time* ta = getStorageLineTime(a);
			const common::Time* tb = getStorageLineTime(b);
			return (ta ? ta->start : 0) < (tb ? tb->start : 0);
		});

		for (auto& line : lyric.lines) {
			std::stable_sort(line.backgrounds.begin(), line.backgrounds.end(), [](const LineBackground& a, const LineBackground& b) {
				const uint32_t sa = a.time.has_value() ? a.time->start : 0;
				const uint32_t sb = b.time.has_value() ? b.time->start : 0;
				return sa < sb;
			});
		}
	}
} // namespace music_lyric_model::storage
