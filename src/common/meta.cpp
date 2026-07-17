#include "common/meta.h"

#include "common/unknown.h"

namespace music_lyric_model::common {
	lyric::common::Meta makeMeta(const lyric::common::Meta& meta) {
		return meta;
	}

	lyric::common::MetaText makeMetaText(const lyric::common::MetaText& text) {
		return text;
	}

	lyric::common::MetaCredit makeMetaCredit(const lyric::common::MetaCredit& credit) {
		return credit;
	}

	lyric::common::MetaReference makeMetaReference(const lyric::common::MetaReference& reference) {
		return reference;
	}

	std::optional<std::string> getMetaText(const google::protobuf::RepeatedPtrField<lyric::common::MetaText>& items, const std::optional<std::string>& language) {
		if (language.has_value()) {
			for (const auto& item : items) {
				if (item.has_language() && item.language() == *language) {
					return item.content();
				}
			}
		}
		return items.empty() ? std::nullopt : std::optional<std::string>(items.Get(0).content());
	}

	std::vector<std::string> getMetaUnknown(const google::protobuf::RepeatedPtrField<lyric::common::Unknown>& unknowns, const std::string& key) {
		return getUnknownValues(unknowns, key);
	}

	std::vector<std::string> getMetaReference(const google::protobuf::RepeatedPtrField<lyric::common::MetaReference>& references, const std::string& platform) {
		for (const auto& item : references) {
			if (item.platform() == platform) {
				return {item.ids().begin(), item.ids().end()};
			}
		}
		return {};
	}
} // namespace music_lyric_model::common
