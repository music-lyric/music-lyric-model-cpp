#include "storage/meta.h"

namespace music_lyric_model::storage {
	lyric::storage::Meta makeMeta(const lyric::storage::Meta& meta) {
		return meta;
	}

	lyric::storage::MetaText makeMetaText(const lyric::storage::MetaText& text) {
		return text;
	}

	lyric::storage::MetaCredit makeMetaCredit(const lyric::storage::MetaCredit& credit) {
		return credit;
	}

	lyric::storage::MetaUnknown makeMetaUnknown(const lyric::storage::MetaUnknown& unknown) {
		return unknown;
	}

	lyric::storage::MetaReference makeMetaReference(const lyric::storage::MetaReference& reference) {
		return reference;
	}

	std::optional<std::string> getMetaText(const google::protobuf::RepeatedPtrField<lyric::storage::MetaText>& items, const std::optional<std::string>& language) {
		if (language.has_value()) {
			for (const auto& item : items) {
				if (item.has_language() && item.language() == *language) {
					return item.content();
				}
			}
		}
		return items.empty() ? std::nullopt : std::optional<std::string>(items.Get(0).content());
	}

	std::vector<std::string> getMetaUnknown(const google::protobuf::RepeatedPtrField<lyric::storage::MetaUnknown>& unknowns, const std::string& key) {
		std::vector<std::string> result;
		for (const auto& item : unknowns) {
			if (item.key() == key) {
				result.push_back(item.value());
			}
		}
		return result;
	}

	std::vector<std::string> getMetaReference(const google::protobuf::RepeatedPtrField<lyric::storage::MetaReference>& references, const std::string& platform) {
		for (const auto& item : references) {
			if (item.platform() == platform) {
				return {item.ids().begin(), item.ids().end()};
			}
		}
		return {};
	}
} // namespace music_lyric_model::storage
