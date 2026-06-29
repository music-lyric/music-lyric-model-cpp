#ifndef MUSIC_LYRIC_LANGUAGE_LANGUAGE_H_
#define MUSIC_LYRIC_LANGUAGE_LANGUAGE_H_

#include <string>

#include "language/language.pb.h"

namespace music_lyric_model {
	using namespace lyric;

	/**
	 * Creates a LanguageItem.
	 */
	lyric::LanguageItem makeLanguageItem();

	/**
	 * Whether a language of the given tag is present.
	 */
	bool hasLanguage(const google::protobuf::RepeatedPtrField<lyric::LanguageItem>& languages, const std::string& tag);

	/**
	 * The language entry of the given tag, null when absent.
	 */
	const lyric::LanguageItem* getLanguageByTag(const google::protobuf::RepeatedPtrField<lyric::LanguageItem>& languages, const std::string& tag);

	/**
	 * The language with the highest share, null when empty.
	 */
	const lyric::LanguageItem* getPrimaryLanguage(const google::protobuf::RepeatedPtrField<lyric::LanguageItem>& languages);
} // namespace music_lyric_model

#endif
