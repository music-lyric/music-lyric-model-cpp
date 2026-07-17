#include "parsed/language.h"

namespace music_lyric_model::parsed {
	lyric::parsed::LanguageItem makeParsedLanguageItem(const lyric::parsed::LanguageItem& item) {
		return item;
	}

	bool hasParsedLanguage(const google::protobuf::RepeatedPtrField<lyric::parsed::LanguageItem>& languages, const std::string& tag) {
		return getParsedLanguageByTag(languages, tag) != nullptr;
	}

	const lyric::parsed::LanguageItem* getParsedLanguageByTag(const google::protobuf::RepeatedPtrField<lyric::parsed::LanguageItem>& languages, const std::string& tag) {
		for (const auto& item : languages) {
			if (item.tag() == tag) {
				return &item;
			}
		}
		return nullptr;
	}

	const lyric::parsed::LanguageItem* getParsedPrimaryLanguage(const google::protobuf::RepeatedPtrField<lyric::parsed::LanguageItem>& languages) {
		const lyric::parsed::LanguageItem* result = nullptr;
		for (const auto& item : languages) {
			if (!result || item.percent() > result->percent()) {
				result = &item;
			}
		}
		return result;
	}
} // namespace music_lyric_model::parsed
