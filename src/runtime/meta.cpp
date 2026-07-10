#include "runtime/meta.h"

namespace music_lyric_model {
	lyric::runtime::Meta makeMeta(const lyric::runtime::Meta& meta) {
		return meta;
	}

	lyric::runtime::MetaText makeMetaText(const lyric::runtime::MetaText& text) {
		return text;
	}

	lyric::runtime::MetaCredit makeMetaCredit(const lyric::runtime::MetaCredit& credit) {
		return credit;
	}

	lyric::runtime::MetaUnknown makeMetaUnknown(const lyric::runtime::MetaUnknown& unknown) {
		return unknown;
	}

	lyric::runtime::MetaReference makeMetaReference(const lyric::runtime::MetaReference& reference) {
		return reference;
	}

	std::optional<std::string> getMetaText(const google::protobuf::RepeatedPtrField<lyric::runtime::MetaText>& items, const std::optional<std::string>& language) {
		if (language.has_value()) {
			for (const auto& item : items) {
				if (item.has_language() && item.language() == *language) {
					return item.content();
				}
			}
		}
		return items.empty() ? std::nullopt : std::optional<std::string>(items.Get(0).content());
	}

	std::vector<std::string> getMetaUnknown(const google::protobuf::RepeatedPtrField<lyric::runtime::MetaUnknown>& unknowns, const std::string& key) {
		std::vector<std::string> result;
		for (const auto& item : unknowns) {
			if (item.key() == key) {
				result.push_back(item.value());
			}
		}
		return result;
	}

	std::vector<std::string> getMetaReference(const google::protobuf::RepeatedPtrField<lyric::runtime::MetaReference>& references, const std::string& platform) {
		for (const auto& item : references) {
			if (item.platform() == platform) {
				return {item.ids().begin(), item.ids().end()};
			}
		}
		return {};
	}
} // namespace music_lyric_model
