#ifndef MUSIC_LYRIC_MODEL_COMMON_META_H_
#define MUSIC_LYRIC_MODEL_COMMON_META_H_

#include <optional>
#include <string>
#include <vector>

#include "model/common/meta.gen.h"

namespace music_lyric_model::common {
	/**
	 * Creates a Meta, the lyric metadata container.
	 */
	Meta makeMeta(Meta init = {});

	/**
	 * Creates a MetaText, a value optionally tagged with a language.
	 */
	MetaText makeMetaText(MetaText init = {});

	/**
	 * Creates a MetaCredit, a role with its credited names.
	 */
	MetaCredit makeMetaCredit(MetaCredit init = {});

	/**
	 * Creates a MetaReference, a platform and its identifiers.
	 */
	MetaReference makeMetaReference(MetaReference init = {});

	/**
	 * Text of a localized meta list, preferring a language match, null when empty.
	 */
	std::optional<std::string> getMetaText(const std::vector<MetaText>& items, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Unrecognized meta values carrying the given original key.
	 */
	std::vector<std::string> getMetaUnknown(const std::vector<Unknown>& unknowns, const std::string& key);

	/**
	 * Reference ids for the given platform.
	 */
	std::vector<std::string> getMetaReference(const std::vector<MetaReference>& references, const std::string& platform);
} // namespace music_lyric_model::common

#endif
