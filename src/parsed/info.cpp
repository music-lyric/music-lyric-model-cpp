#include "parsed/info.h"

#include <algorithm>
#include <stdexcept>

#include "google/protobuf/util/json_util.h"
#include "parsed/info.pb.h"
#include "parsed/language.pb.h"
#include "parsed/line.h"
#include "parsed/line.pb.h"
#include "pb.h"
#include "version.h"

namespace music_lyric_model::parsed {
	namespace {
		/**
		 * Convert a public LanguageItem to its protobuf form.
		 */
		lyric::parsed::LanguageItem toPb(const LanguageItem& item) {
			lyric::parsed::LanguageItem out;
			out.set_tag(item.tag);
			out.set_percent(item.percent);
			return out;
		}

		/**
		 * Convert a protobuf LanguageItem to the public form.
		 */
		LanguageItem fromPb(const lyric::parsed::LanguageItem& item) {
			return LanguageItem{item.tag(), item.percent()};
		}

		/**
		 * Convert a public LineBackground to its protobuf form.
		 */
		lyric::parsed::LineBackground toPb(const LineBackground& line) {
			lyric::parsed::LineBackground out;
			internal::putMap(line.extra, out.mutable_extra());
			if (line.time.has_value()) {
				*out.mutable_time() = internal::toPb(*line.time);
			}
			for (const auto& agent : line.agents) {
				out.add_agents(agent);
			}
			for (const auto& language : line.languages) {
				out.add_languages(language);
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
		LineBackground fromPb(const lyric::parsed::LineBackground& line) {
			LineBackground out;
			out.extra = internal::takeMap(line.extra());
			out.time  = internal::optionalTimeFrom(line.has_time(), line.time());
			out.agents.assign(line.agents().begin(), line.agents().end());
			out.languages.assign(line.languages().begin(), line.languages().end());
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
		 * Convert a public LineNormal to its protobuf form.
		 */
		lyric::parsed::LineNormal toPb(const LineNormal& line) {
			lyric::parsed::LineNormal out;
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
			for (const auto& language : line.languages) {
				out.add_languages(language);
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
		 * Convert a protobuf LineNormal to the public form.
		 */
		LineNormal fromPb(const lyric::parsed::LineNormal& line) {
			LineNormal out;
			out.extra = internal::takeMap(line.extra());
			out.time  = internal::optionalTimeFrom(line.has_time(), line.time());
			if (line.has_part()) {
				out.part = internal::fromPb(line.part());
			}
			out.agents.assign(line.agents().begin(), line.agents().end());
			out.languages.assign(line.languages().begin(), line.languages().end());
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
		 * Convert a public LineInterlude to its protobuf form.
		 */
		lyric::parsed::LineInterlude toPb(const LineInterlude& line) {
			lyric::parsed::LineInterlude out;
			internal::putMap(line.extra, out.mutable_extra());
			if (line.time.has_value()) {
				*out.mutable_time() = internal::toPb(*line.time);
			}
			return out;
		}

		/**
		 * Convert a protobuf LineInterlude to the public form.
		 */
		LineInterlude fromPb(const lyric::parsed::LineInterlude& line) {
			LineInterlude out;
			out.extra = internal::takeMap(line.extra());
			out.time  = internal::optionalTimeFrom(line.has_time(), line.time());
			return out;
		}

		/**
		 * Convert a public Line to its protobuf form.
		 */
		lyric::parsed::Line toPb(const Line& line) {
			lyric::parsed::Line out;
			if (const LineNormal* normal = asParsedLineNormal(line)) {
				*out.mutable_normal() = toPb(*normal);
				return out;
			}
			if (const LineInterlude* interlude = asParsedLineInterlude(line)) {
				*out.mutable_interlude() = toPb(*interlude);
				return out;
			}
			throw std::runtime_error("parsed line body is unset");
		}

		/**
		 * Convert a protobuf Line to the public form.
		 */
		Line fromPb(const lyric::parsed::Line& line) {
			switch (line.body_case()) {
			case lyric::parsed::Line::kNormal:
				return Line{fromPb(line.normal())};
			case lyric::parsed::Line::kInterlude:
				return Line{fromPb(line.interlude())};
			case lyric::parsed::Line::BODY_NOT_SET:
			default:
				throw std::runtime_error("parsed line body is unset");
			}
		}

		/**
		 * Convert a public Info to its protobuf form.
		 */
		lyric::parsed::Info toPb(const Info& info) {
			lyric::parsed::Info out;
			out.set_version(info.version);
			switch (info.type) {
			case InfoType::Invalid:
				out.set_type(lyric::parsed::INFO_TYPE_INVALID);
				break;
			case InfoType::Valid:
				out.set_type(lyric::parsed::INFO_TYPE_VALID);
				break;
			case InfoType::Unspecified:
			default:
				out.set_type(lyric::parsed::INFO_TYPE_UNSPECIFIED);
				break;
			}
			out.set_timing(internal::toPb(info.timing));
			internal::putMap(info.extra, out.mutable_extra());
			*out.mutable_meta() = internal::toPb(info.meta);
			for (const auto& item : info.languages) {
				*out.add_languages() = toPb(item);
			}
			for (const auto& item : info.agents) {
				*out.add_agents() = internal::toPb(item);
			}
			for (const auto& item : info.lines) {
				*out.add_lines() = toPb(item);
			}
			return out;
		}

		/**
		 * Convert a protobuf Info to the public form.
		 */
		Info fromPb(const lyric::parsed::Info& info) {
			Info out;
			out.version = info.version();
			switch (info.type()) {
			case lyric::parsed::INFO_TYPE_INVALID:
				out.type = InfoType::Invalid;
				break;
			case lyric::parsed::INFO_TYPE_VALID:
				out.type = InfoType::Valid;
				break;
			case lyric::parsed::INFO_TYPE_UNSPECIFIED:
			default:
				out.type = InfoType::Unspecified;
				break;
			}
			out.timing = internal::fromPb(info.timing());
			out.extra  = internal::takeMap(info.extra());
			if (info.has_meta()) {
				out.meta = internal::fromPb(info.meta());
			}
			out.languages.reserve(static_cast<size_t>(info.languages_size()));
			for (const auto& item : info.languages()) {
				out.languages.push_back(fromPb(item));
			}
			out.agents.reserve(static_cast<size_t>(info.agents_size()));
			for (const auto& item : info.agents()) {
				out.agents.push_back(internal::fromPb(item));
			}
			out.lines.reserve(static_cast<size_t>(info.lines_size()));
			for (const auto& item : info.lines()) {
				out.lines.push_back(fromPb(item));
			}
			return out;
		}
	} // namespace

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
		const lyric::parsed::Info pb = toPb(info);
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
		return fromPb(pb);
	}

	std::string parsedInfoToJson(const Info& info) {
		const lyric::parsed::Info pb = toPb(info);
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
		return fromPb(pb);
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
