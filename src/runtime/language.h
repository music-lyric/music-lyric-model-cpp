#ifndef MUSIC_LYRIC_RUNTIME_LANGUAGE_H_
#define MUSIC_LYRIC_RUNTIME_LANGUAGE_H_

#include <string>

#include "runtime/language/language.pb.h"

namespace music_lyric_model::runtime {
	using namespace lyric::runtime;

	/**
	 * Creates a LanguageItem.
	 */
	lyric::runtime::LanguageItem makeLanguageItem(const lyric::runtime::LanguageItem& item = {});

	/**
	 * Whether a language of the given tag is present.
	 */
	bool hasLanguage(const google::protobuf::RepeatedPtrField<lyric::runtime::LanguageItem>& languages, const std::string& tag);

	/**
	 * The language entry of the given tag, null when absent.
	 */
	const lyric::runtime::LanguageItem* getLanguageByTag(const google::protobuf::RepeatedPtrField<lyric::runtime::LanguageItem>& languages, const std::string& tag);

	/**
	 * The language with the highest share, null when empty.
	 */
	const lyric::runtime::LanguageItem* getPrimaryLanguage(const google::protobuf::RepeatedPtrField<lyric::runtime::LanguageItem>& languages);
} // namespace music_lyric_model::runtime

#endif
