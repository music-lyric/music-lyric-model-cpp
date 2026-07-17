#include "common/meta.h"

#include "common/unknown.h"

namespace music_lyric_model::common {
	std::optional<std::string> getMetaText(const std::vector<MetaText>& items, const std::optional<std::string>& language) {
		if (language.has_value()) {
			for (const auto& item : items) {
				if (item.language.has_value() && *item.language == *language) {
					return item.content;
				}
			}
		}
		return items.empty() ? std::nullopt : std::optional<std::string>(items[0].content);
	}

	std::vector<std::string> getMetaUnknown(const std::vector<Unknown>& unknowns, const std::string& key) {
		return getUnknownValues(unknowns, key);
	}

	std::vector<std::string> getMetaReference(const std::vector<MetaReference>& references, const std::string& platform) {
		for (const auto& item : references) {
			if (item.platform == platform) {
				return item.ids;
			}
		}
		return {};
	}
} // namespace music_lyric_model::common
