#include "language/language.h"

namespace music_lyric_model {
	lyric::LanguageItem makeLanguageItem() {
		return {};
	}

	bool hasLanguage(const google::protobuf::RepeatedPtrField<lyric::LanguageItem> &languages, const std::string &tag) {
		for (const auto &item : languages) {
			if (item.tag() == tag) {
				return true;
			}
		}
		return false;
	}

	const lyric::LanguageItem *getLanguageByTag(const google::protobuf::RepeatedPtrField<lyric::LanguageItem> &languages, const std::string &tag) {
		for (const auto &item : languages) {
			if (item.tag() == tag) {
				return &item;
			}
		}
		return nullptr;
	}

	const lyric::LanguageItem *getPrimaryLanguage(const google::protobuf::RepeatedPtrField<lyric::LanguageItem> &languages) {
		const lyric::LanguageItem *result = nullptr;
		for (const auto &item : languages) {
			if (!result || item.percent() > result->percent()) {
				result = &item;
			}
		}
		return result;
	}
}
