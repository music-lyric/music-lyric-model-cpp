#include "parsed/language.h"

namespace music_lyric_model::parsed {
	bool hasParsedLanguage(const std::vector<LanguageItem>& languages, const std::string& tag) {
		return getParsedLanguageByTag(languages, tag) != nullptr;
	}

	const LanguageItem* getParsedLanguageByTag(const std::vector<LanguageItem>& languages, const std::string& tag) {
		for (const auto& item : languages) {
			if (item.tag == tag) {
				return &item;
			}
		}
		return nullptr;
	}

	const LanguageItem* getParsedPrimaryLanguage(const std::vector<LanguageItem>& languages) {
		const LanguageItem* result = nullptr;
		for (const auto& item : languages) {
			if (!result || item.percent > result->percent) {
				result = &item;
			}
		}
		return result;
	}
} // namespace music_lyric_model::parsed
