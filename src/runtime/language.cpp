#include "runtime/language.h"

namespace music_lyric_model::runtime {
	lyric::runtime::LanguageItem makeLanguageItem(const lyric::runtime::LanguageItem& item) {
		return item;
	}

	bool hasLanguage(const google::protobuf::RepeatedPtrField<lyric::runtime::LanguageItem>& languages, const std::string& tag) {
		for (const auto& item : languages) {
			if (item.tag() == tag) {
				return true;
			}
		}
		return false;
	}

	const lyric::runtime::LanguageItem* getLanguageByTag(const google::protobuf::RepeatedPtrField<lyric::runtime::LanguageItem>& languages, const std::string& tag) {
		for (const auto& item : languages) {
			if (item.tag() == tag) {
				return &item;
			}
		}
		return nullptr;
	}

	const lyric::runtime::LanguageItem* getPrimaryLanguage(const google::protobuf::RepeatedPtrField<lyric::runtime::LanguageItem>& languages) {
		const lyric::runtime::LanguageItem* result = nullptr;
		for (const auto& item : languages) {
			if (!result || item.percent() > result->percent()) {
				result = &item;
			}
		}
		return result;
	}
} // namespace music_lyric_model::runtime
