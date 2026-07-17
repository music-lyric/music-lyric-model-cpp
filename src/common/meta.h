#ifndef MUSIC_LYRIC_COMMON_META_H_
#define MUSIC_LYRIC_COMMON_META_H_

#include <optional>
#include <string>
#include <vector>

#include "common/meta.pb.h"
#include "common/unknown.pb.h"

namespace music_lyric_model::common {
	/**
	 * Creates a Meta, the lyric metadata container.
	 */
	lyric::common::Meta makeMeta(const lyric::common::Meta& meta = {});

	/**
	 * Creates a MetaText, a value optionally tagged with a language.
	 */
	lyric::common::MetaText makeMetaText(const lyric::common::MetaText& text = {});

	/**
	 * Creates a MetaCredit, a role with its credited names.
	 */
	lyric::common::MetaCredit makeMetaCredit(const lyric::common::MetaCredit& credit = {});

	/**
	 * Creates a MetaReference, a platform and its identifiers.
	 */
	lyric::common::MetaReference makeMetaReference(const lyric::common::MetaReference& reference = {});

	/**
	 * Text of a localized meta list, preferring a language match, null when empty.
	 */
	std::optional<std::string> getMetaText(const google::protobuf::RepeatedPtrField<lyric::common::MetaText>& items, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Unrecognized meta values carrying the given original key.
	 */
	std::vector<std::string> getMetaUnknown(const google::protobuf::RepeatedPtrField<lyric::common::Unknown>& unknowns, const std::string& key);

	/**
	 * Reference ids for the given platform.
	 */
	std::vector<std::string> getMetaReference(const google::protobuf::RepeatedPtrField<lyric::common::MetaReference>& references, const std::string& platform);
} // namespace music_lyric_model::common

#endif
