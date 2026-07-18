#ifndef MUSIC_LYRIC_MODEL_PARSED_LANGUAGE_H_
#define MUSIC_LYRIC_MODEL_PARSED_LANGUAGE_H_

#include <string>
#include <vector>

#include "model/parsed/language.gen.h"

namespace music_lyric_model::parsed {
	/**
	 * Creates a LanguageItem.
	 */
	LanguageItem makeParsedLanguageItem(LanguageItem init = {});

	/**
	 * Whether a language of the given tag is present.
	 */
	bool hasParsedLanguage(const std::vector<LanguageItem>& languages, const std::string& tag);

	/**
	 * The language entry of the given tag, null when absent.
	 */
	const LanguageItem* getParsedLanguageByTag(const std::vector<LanguageItem>& languages, const std::string& tag);

	/**
	 * The language with the highest share, null when empty.
	 */
	const LanguageItem* getParsedPrimaryLanguage(const std::vector<LanguageItem>& languages);
} // namespace music_lyric_model::parsed

#endif
