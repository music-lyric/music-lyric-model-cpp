#include "meta/meta.h"

namespace music_lyric_model {
	lyric::Meta makeMeta() {
		return {};
	}

	lyric::MetaText makeMetaText() {
		return {};
	}

	lyric::MetaCredit makeMetaCredit() {
		return {};
	}

	lyric::MetaUnknown makeMetaUnknown() {
		return {};
	}

	lyric::MetaReference makeMetaReference() {
		return {};
	}

	std::optional<std::string> getMetaText(const google::protobuf::RepeatedPtrField<lyric::MetaText>& items, const std::optional<std::string>& language) {
		if (language.has_value()) {
			for (const auto& item : items) {
				if (item.has_language() && item.language() == *language) {
					return item.value();
				}
			}
		}
		return items.empty() ? std::nullopt : std::optional<std::string>(items.Get(0).value());
	}

	std::vector<std::string> getMetaUnknown(const google::protobuf::RepeatedPtrField<lyric::MetaUnknown>& unknowns, const std::string& key) {
		std::vector<std::string> result;
		for (const auto& item : unknowns) {
			if (item.key() == key) {
				result.push_back(item.value());
			}
		}
		return result;
	}

	std::vector<std::string> getMetaReference(const google::protobuf::RepeatedPtrField<lyric::MetaReference>& references, const std::string& platform) {
		for (const auto& item : references) {
			if (item.platform() == platform) {
				return {item.ids().begin(), item.ids().end()};
			}
		}
		return {};
	}
} // namespace music_lyric_model
