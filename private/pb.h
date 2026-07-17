#ifndef MUSIC_LYRIC_MODEL_PRIVATE_PB_COMMON_H_
#define MUSIC_LYRIC_MODEL_PRIVATE_PB_COMMON_H_

// Internal protobuf bridging. Not part of the public API.

#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "common/agent.pb.h"
#include "common/line.pb.h"
#include "common/meta.pb.h"
#include "common/part.pb.h"
#include "common/time.pb.h"
#include "common/timing.pb.h"
#include "common/unknown.pb.h"
#include "common/word.pb.h"

#include "common/agent.h"
#include "common/common.h"
#include "common/line.h"
#include "common/meta.h"
#include "common/part.h"
#include "common/time.h"
#include "common/unknown.h"
#include "common/word.h"

namespace music_lyric_model::internal {
	inline void putMap(const std::unordered_map<std::string, std::string>& src, google::protobuf::Map<std::string, std::string>* dst) {
		dst->clear();
		for (const auto& [k, v] : src) {
			(*dst)[k] = v;
		}
	}

	inline std::unordered_map<std::string, std::string> takeMap(const google::protobuf::Map<std::string, std::string>& src) {
		std::unordered_map<std::string, std::string> out;
		out.reserve(static_cast<size_t>(src.size()));
		for (const auto& [k, v] : src) {
			out.emplace(k, v);
		}
		return out;
	}

	inline lyric::common::Time toPb(const common::Time& time) {
		lyric::common::Time out;
		out.set_start(time.start);
		out.set_end(time.end);
		return out;
	}

	inline common::Time fromPb(const lyric::common::Time& time) {
		return common::Time{time.start(), time.end()};
	}

	inline std::optional<common::Time> optionalTimeFrom(bool has, const lyric::common::Time& time) {
		return has ? std::optional<common::Time>(fromPb(time)) : std::nullopt;
	}

	inline lyric::common::Unknown toPb(const common::Unknown& item) {
		lyric::common::Unknown out;
		out.set_key(item.key);
		out.set_value(item.value);
		return out;
	}

	inline common::Unknown fromPb(const lyric::common::Unknown& item) {
		return common::Unknown{item.key(), item.value()};
	}

	inline lyric::common::Timing toPb(common::Timing timing) {
		switch (timing) {
		case common::Timing::None:
			return lyric::common::TIMING_NONE;
		case common::Timing::Line:
			return lyric::common::TIMING_LINE;
		case common::Timing::Word:
			return lyric::common::TIMING_WORD;
		case common::Timing::Unspecified:
		default:
			return lyric::common::TIMING_UNSPECIFIED;
		}
	}

	inline common::Timing fromPb(lyric::common::Timing timing) {
		switch (timing) {
		case lyric::common::TIMING_NONE:
			return common::Timing::None;
		case lyric::common::TIMING_LINE:
			return common::Timing::Line;
		case lyric::common::TIMING_WORD:
			return common::Timing::Word;
		case lyric::common::TIMING_UNSPECIFIED:
		default:
			return common::Timing::Unspecified;
		}
	}

	inline lyric::common::AgentType toPb(common::AgentType type) {
		switch (type) {
		case common::AgentType::Unknown:
			return lyric::common::AGENT_TYPE_UNKNOWN;
		case common::AgentType::Person:
			return lyric::common::AGENT_TYPE_PERSON;
		case common::AgentType::Group:
			return lyric::common::AGENT_TYPE_GROUP;
		case common::AgentType::Other:
			return lyric::common::AGENT_TYPE_OTHER;
		case common::AgentType::Unspecified:
		default:
			return lyric::common::AGENT_TYPE_UNSPECIFIED;
		}
	}

	inline common::AgentType fromPb(lyric::common::AgentType type) {
		switch (type) {
		case lyric::common::AGENT_TYPE_UNKNOWN:
			return common::AgentType::Unknown;
		case lyric::common::AGENT_TYPE_PERSON:
			return common::AgentType::Person;
		case lyric::common::AGENT_TYPE_GROUP:
			return common::AgentType::Group;
		case lyric::common::AGENT_TYPE_OTHER:
			return common::AgentType::Other;
		case lyric::common::AGENT_TYPE_UNSPECIFIED:
		default:
			return common::AgentType::Unspecified;
		}
	}

	inline lyric::common::AgentItem toPb(const common::AgentItem& item) {
		lyric::common::AgentItem out;
		out.set_id(item.id);
		out.set_type(toPb(item.type));
		for (const auto& name : item.names) {
			out.add_names(name);
		}
		return out;
	}

	inline common::AgentItem fromPb(const lyric::common::AgentItem& item) {
		common::AgentItem out;
		out.id   = item.id();
		out.type = fromPb(item.type());
		out.names.assign(item.names().begin(), item.names().end());
		return out;
	}

	inline lyric::common::PartType toPb(common::PartType type) {
		switch (type) {
		case common::PartType::Other:
			return lyric::common::PART_TYPE_OTHER;
		case common::PartType::Intro:
			return lyric::common::PART_TYPE_INTRO;
		case common::PartType::Verse:
			return lyric::common::PART_TYPE_VERSE;
		case common::PartType::PreChorus:
			return lyric::common::PART_TYPE_PRE_CHORUS;
		case common::PartType::Chorus:
			return lyric::common::PART_TYPE_CHORUS;
		case common::PartType::PostChorus:
			return lyric::common::PART_TYPE_POST_CHORUS;
		case common::PartType::Bridge:
			return lyric::common::PART_TYPE_BRIDGE;
		case common::PartType::Refrain:
			return lyric::common::PART_TYPE_REFRAIN;
		case common::PartType::Instrumental:
			return lyric::common::PART_TYPE_INSTRUMENTAL;
		case common::PartType::Outro:
			return lyric::common::PART_TYPE_OUTRO;
		case common::PartType::Unspecified:
		default:
			return lyric::common::PART_TYPE_UNSPECIFIED;
		}
	}

	inline common::PartType fromPb(lyric::common::PartType type) {
		switch (type) {
		case lyric::common::PART_TYPE_OTHER:
			return common::PartType::Other;
		case lyric::common::PART_TYPE_INTRO:
			return common::PartType::Intro;
		case lyric::common::PART_TYPE_VERSE:
			return common::PartType::Verse;
		case lyric::common::PART_TYPE_PRE_CHORUS:
			return common::PartType::PreChorus;
		case lyric::common::PART_TYPE_CHORUS:
			return common::PartType::Chorus;
		case lyric::common::PART_TYPE_POST_CHORUS:
			return common::PartType::PostChorus;
		case lyric::common::PART_TYPE_BRIDGE:
			return common::PartType::Bridge;
		case lyric::common::PART_TYPE_REFRAIN:
			return common::PartType::Refrain;
		case lyric::common::PART_TYPE_INSTRUMENTAL:
			return common::PartType::Instrumental;
		case lyric::common::PART_TYPE_OUTRO:
			return common::PartType::Outro;
		case lyric::common::PART_TYPE_UNSPECIFIED:
		default:
			return common::PartType::Unspecified;
		}
	}

	inline lyric::common::Part toPb(const common::Part& part) {
		lyric::common::Part out;
		out.set_type(toPb(part.type));
		if (part.label.has_value()) {
			out.set_label(*part.label);
		}
		return out;
	}

	inline common::Part fromPb(const lyric::common::Part& part) {
		common::Part out;
		out.type = fromPb(part.type());
		if (part.has_label()) {
			out.label = part.label();
		}
		return out;
	}

	inline lyric::common::MetaText toPb(const common::MetaText& text) {
		lyric::common::MetaText out;
		if (text.language.has_value()) {
			out.set_language(*text.language);
		}
		out.set_content(text.content);
		return out;
	}

	inline common::MetaText fromPb(const lyric::common::MetaText& text) {
		common::MetaText out;
		if (text.has_language()) {
			out.language = text.language();
		}
		out.content = text.content();
		return out;
	}

	inline lyric::common::MetaCredit toPb(const common::MetaCredit& credit) {
		lyric::common::MetaCredit out;
		out.set_role(credit.role);
		for (const auto& name : credit.names) {
			*out.add_names() = toPb(name);
		}
		return out;
	}

	inline common::MetaCredit fromPb(const lyric::common::MetaCredit& credit) {
		common::MetaCredit out;
		out.role = credit.role();
		out.names.reserve(static_cast<size_t>(credit.names_size()));
		for (const auto& name : credit.names()) {
			out.names.push_back(fromPb(name));
		}
		return out;
	}

	inline lyric::common::MetaReference toPb(const common::MetaReference& reference) {
		lyric::common::MetaReference out;
		out.set_platform(reference.platform);
		for (const auto& id : reference.ids) {
			out.add_ids(id);
		}
		return out;
	}

	inline common::MetaReference fromPb(const lyric::common::MetaReference& reference) {
		common::MetaReference out;
		out.platform = reference.platform();
		out.ids.assign(reference.ids().begin(), reference.ids().end());
		return out;
	}

	inline lyric::common::Meta toPb(const common::Meta& meta) {
		lyric::common::Meta out;
		for (const auto& item : meta.unknowns) {
			*out.add_unknowns() = toPb(item);
		}
		out.set_offset(meta.offset);
		out.set_duration(meta.duration);
		for (const auto& item : meta.titles) {
			*out.add_titles() = toPb(item);
		}
		for (const auto& item : meta.artists) {
			*out.add_artists() = toPb(item);
		}
		for (const auto& item : meta.albums) {
			*out.add_albums() = toPb(item);
		}
		for (const auto& item : meta.authors) {
			*out.add_authors() = toPb(item);
		}
		for (const auto& item : meta.isrcs) {
			out.add_isrcs(item);
		}
		for (const auto& item : meta.credits) {
			*out.add_credits() = toPb(item);
		}
		for (const auto& item : meta.references) {
			*out.add_references() = toPb(item);
		}
		return out;
	}

	inline common::Meta fromPb(const lyric::common::Meta& meta) {
		common::Meta out;
		out.unknowns.reserve(static_cast<size_t>(meta.unknowns_size()));
		for (const auto& item : meta.unknowns()) {
			out.unknowns.push_back(fromPb(item));
		}
		out.offset   = meta.offset();
		out.duration = meta.duration();
		out.titles.reserve(static_cast<size_t>(meta.titles_size()));
		for (const auto& item : meta.titles()) {
			out.titles.push_back(fromPb(item));
		}
		out.artists.reserve(static_cast<size_t>(meta.artists_size()));
		for (const auto& item : meta.artists()) {
			out.artists.push_back(fromPb(item));
		}
		out.albums.reserve(static_cast<size_t>(meta.albums_size()));
		for (const auto& item : meta.albums()) {
			out.albums.push_back(fromPb(item));
		}
		out.authors.reserve(static_cast<size_t>(meta.authors_size()));
		for (const auto& item : meta.authors()) {
			out.authors.push_back(fromPb(item));
		}
		out.isrcs.assign(meta.isrcs().begin(), meta.isrcs().end());
		out.credits.reserve(static_cast<size_t>(meta.credits_size()));
		for (const auto& item : meta.credits()) {
			out.credits.push_back(fromPb(item));
		}
		out.references.reserve(static_cast<size_t>(meta.references_size()));
		for (const auto& item : meta.references()) {
			out.references.push_back(fromPb(item));
		}
		return out;
	}

	inline lyric::common::LineAnnotationRoman toPb(const common::LineAnnotationRoman& item) {
		lyric::common::LineAnnotationRoman out;
		if (item.language.has_value()) {
			out.set_language(*item.language);
		}
		out.set_content(item.content);
		return out;
	}

	inline common::LineAnnotationRoman fromPb(const lyric::common::LineAnnotationRoman& item) {
		common::LineAnnotationRoman out;
		if (item.has_language()) {
			out.language = item.language();
		}
		out.content = item.content();
		return out;
	}

	inline lyric::common::LineAnnotationTranslation toPb(const common::LineAnnotationTranslation& item) {
		lyric::common::LineAnnotationTranslation out;
		if (item.language.has_value()) {
			out.set_language(*item.language);
		}
		out.set_content(item.content);
		return out;
	}

	inline common::LineAnnotationTranslation fromPb(const lyric::common::LineAnnotationTranslation& item) {
		common::LineAnnotationTranslation out;
		if (item.has_language()) {
			out.language = item.language();
		}
		out.content = item.content();
		return out;
	}

	inline lyric::common::LineAnnotation toPb(const common::LineAnnotation& annotation) {
		lyric::common::LineAnnotation out;
		for (const auto& item : annotation.unknowns) {
			*out.add_unknowns() = toPb(item);
		}
		for (const auto& item : annotation.romans) {
			*out.add_romans() = toPb(item);
		}
		for (const auto& item : annotation.translations) {
			*out.add_translations() = toPb(item);
		}
		return out;
	}

	inline common::LineAnnotation fromPb(const lyric::common::LineAnnotation& annotation) {
		common::LineAnnotation out;
		out.unknowns.reserve(static_cast<size_t>(annotation.unknowns_size()));
		for (const auto& item : annotation.unknowns()) {
			out.unknowns.push_back(fromPb(item));
		}
		out.romans.reserve(static_cast<size_t>(annotation.romans_size()));
		for (const auto& item : annotation.romans()) {
			out.romans.push_back(fromPb(item));
		}
		out.translations.reserve(static_cast<size_t>(annotation.translations_size()));
		for (const auto& item : annotation.translations()) {
			out.translations.push_back(fromPb(item));
		}
		return out;
	}

	inline lyric::common::WordAnnotationContent toPb(const common::WordAnnotationContent& item) {
		lyric::common::WordAnnotationContent out;
		if (item.time.has_value()) {
			*out.mutable_time() = toPb(*item.time);
		}
		out.set_content(item.content);
		return out;
	}

	inline common::WordAnnotationContent fromPb(const lyric::common::WordAnnotationContent& item) {
		common::WordAnnotationContent out;
		out.time    = optionalTimeFrom(item.has_time(), item.time());
		out.content = item.content();
		return out;
	}

	inline lyric::common::WordAnnotationTranslation toPb(const common::WordAnnotationTranslation& item) {
		lyric::common::WordAnnotationTranslation out;
		if (item.language.has_value()) {
			out.set_language(*item.language);
		}
		out.set_content(item.content);
		return out;
	}

	inline common::WordAnnotationTranslation fromPb(const lyric::common::WordAnnotationTranslation& item) {
		common::WordAnnotationTranslation out;
		if (item.has_language()) {
			out.language = item.language();
		}
		out.content = item.content();
		return out;
	}

	inline lyric::common::WordAnnotationRoman toPb(const common::WordAnnotationRoman& item) {
		lyric::common::WordAnnotationRoman out;
		if (item.time.has_value()) {
			*out.mutable_time() = toPb(*item.time);
		}
		if (item.language.has_value()) {
			out.set_language(*item.language);
		}
		for (const auto& word : item.words) {
			*out.add_words() = toPb(word);
		}
		return out;
	}

	inline common::WordAnnotationRoman fromPb(const lyric::common::WordAnnotationRoman& item) {
		common::WordAnnotationRoman out;
		out.time = optionalTimeFrom(item.has_time(), item.time());
		if (item.has_language()) {
			out.language = item.language();
		}
		out.words.reserve(static_cast<size_t>(item.words_size()));
		for (const auto& word : item.words()) {
			out.words.push_back(fromPb(word));
		}
		return out;
	}

	inline lyric::common::WordAnnotationRuby toPb(const common::WordAnnotationRuby& item) {
		lyric::common::WordAnnotationRuby out;
		if (item.time.has_value()) {
			*out.mutable_time() = toPb(*item.time);
		}
		if (item.language.has_value()) {
			out.set_language(*item.language);
		}
		for (const auto& word : item.words) {
			*out.add_words() = toPb(word);
		}
		out.set_phrase_start(item.phraseStart);
		return out;
	}

	inline common::WordAnnotationRuby fromPb(const lyric::common::WordAnnotationRuby& item) {
		common::WordAnnotationRuby out;
		out.time = optionalTimeFrom(item.has_time(), item.time());
		if (item.has_language()) {
			out.language = item.language();
		}
		out.words.reserve(static_cast<size_t>(item.words_size()));
		for (const auto& word : item.words()) {
			out.words.push_back(fromPb(word));
		}
		out.phraseStart = item.phrase_start();
		return out;
	}

	inline lyric::common::WordAnnotation toPb(const common::WordAnnotation& annotation) {
		lyric::common::WordAnnotation out;
		for (const auto& item : annotation.unknowns) {
			*out.add_unknowns() = toPb(item);
		}
		for (const auto& item : annotation.rubies) {
			*out.add_rubies() = toPb(item);
		}
		for (const auto& item : annotation.romans) {
			*out.add_romans() = toPb(item);
		}
		for (const auto& item : annotation.translations) {
			*out.add_translations() = toPb(item);
		}
		return out;
	}

	inline common::WordAnnotation fromPb(const lyric::common::WordAnnotation& annotation) {
		common::WordAnnotation out;
		out.unknowns.reserve(static_cast<size_t>(annotation.unknowns_size()));
		for (const auto& item : annotation.unknowns()) {
			out.unknowns.push_back(fromPb(item));
		}
		out.rubies.reserve(static_cast<size_t>(annotation.rubies_size()));
		for (const auto& item : annotation.rubies()) {
			out.rubies.push_back(fromPb(item));
		}
		out.romans.reserve(static_cast<size_t>(annotation.romans_size()));
		for (const auto& item : annotation.romans()) {
			out.romans.push_back(fromPb(item));
		}
		out.translations.reserve(static_cast<size_t>(annotation.translations_size()));
		for (const auto& item : annotation.translations()) {
			out.translations.push_back(fromPb(item));
		}
		return out;
	}

	inline lyric::common::WordNormal toPb(const common::WordNormal& word) {
		lyric::common::WordNormal out;
		putMap(word.extra, out.mutable_extra());
		if (word.time.has_value()) {
			*out.mutable_time() = toPb(*word.time);
		}
		out.set_content(word.content);
		if (word.language.has_value()) {
			out.set_language(*word.language);
		}
		if (word.annotation.has_value()) {
			*out.mutable_annotation() = toPb(*word.annotation);
		}
		out.set_stress(word.stress);
		return out;
	}

	inline common::WordNormal fromPb(const lyric::common::WordNormal& word) {
		common::WordNormal out;
		out.extra = takeMap(word.extra());
		out.time  = optionalTimeFrom(word.has_time(), word.time());
		out.content = word.content();
		if (word.has_language()) {
			out.language = word.language();
		}
		if (word.has_annotation()) {
			out.annotation = fromPb(word.annotation());
		}
		out.stress = word.stress();
		return out;
	}

	inline lyric::common::WordSpace toPb(const common::WordSpace& space) {
		lyric::common::WordSpace out;
		out.set_count(space.count);
		return out;
	}

	inline common::WordSpace fromPb(const lyric::common::WordSpace& space) {
		return common::WordSpace{space.count()};
	}

	inline lyric::common::Word toPb(const common::Word& word) {
		lyric::common::Word out;
		if (const common::WordNormal* normal = std::get_if<common::WordNormal>(&word)) {
			*out.mutable_normal() = toPb(*normal);
			return out;
		}
		if (const common::WordSpace* space = std::get_if<common::WordSpace>(&word)) {
			*out.mutable_space() = toPb(*space);
			return out;
		}
		throw std::runtime_error("word body is unset");
	}

	inline common::Word fromPb(const lyric::common::Word& word) {
		switch (word.body_case()) {
		case lyric::common::Word::kNormal:
			return common::Word{fromPb(word.normal())};
		case lyric::common::Word::kSpace:
			return common::Word{fromPb(word.space())};
		case lyric::common::Word::BODY_NOT_SET:
		default:
			throw std::runtime_error("word body is unset");
		}
	}
} // namespace music_lyric_model::internal

#endif
