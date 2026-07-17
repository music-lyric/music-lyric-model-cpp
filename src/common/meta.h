#ifndef MUSIC_LYRIC_MODEL_COMMON_META_H_
#define MUSIC_LYRIC_MODEL_COMMON_META_H_

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "unknown.h"

namespace music_lyric_model::common {
	/**
	 * A value optionally tagged with a language.
	 */
	struct MetaText {
		std::optional<std::string> language;
		std::string                content;
	};

	/**
	 * A role with its credited names.
	 */
	struct MetaCredit {
		std::string           role;
		std::vector<MetaText> names;
	};

	/**
	 * A platform and its identifiers.
	 */
	struct MetaReference {
		std::string              platform;
		std::vector<std::string> ids;
	};

	/**
	 * The lyric metadata container.
	 */
	struct Meta {
		std::vector<Unknown>       unknowns;
		int32_t                    offset   = 0;
		uint32_t                   duration = 0;
		std::vector<MetaText>      titles;
		std::vector<MetaText>      artists;
		std::vector<MetaText>      albums;
		std::vector<MetaText>      authors;
		std::vector<std::string>   isrcs;
		std::vector<MetaCredit>    credits;
		std::vector<MetaReference> references;
	};

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
