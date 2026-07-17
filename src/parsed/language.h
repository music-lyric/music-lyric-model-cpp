#ifndef MUSIC_LYRIC_MODEL_PARSED_LANGUAGE_H_
#define MUSIC_LYRIC_MODEL_PARSED_LANGUAGE_H_

#include <string>

#include "parsed/language.pb.h"

namespace music_lyric_model::parsed {
	/**
	 * Creates a LanguageItem.
	 */
	lyric::parsed::LanguageItem makeParsedLanguageItem(const lyric::parsed::LanguageItem& item = {});

	/**
	 * Whether a language of the given tag is present.
	 */
	bool hasParsedLanguage(const google::protobuf::RepeatedPtrField<lyric::parsed::LanguageItem>& languages, const std::string& tag);

	/**
	 * The language entry of the given tag, null when absent.
	 */
	const lyric::parsed::LanguageItem* getParsedLanguageByTag(const google::protobuf::RepeatedPtrField<lyric::parsed::LanguageItem>& languages, const std::string& tag);

	/**
	 * The language with the highest share, null when empty.
	 */
	const lyric::parsed::LanguageItem* getParsedPrimaryLanguage(const google::protobuf::RepeatedPtrField<lyric::parsed::LanguageItem>& languages);
} // namespace music_lyric_model::parsed

#endif
