#ifndef MUSIC_LYRIC_STORAGE_META_H_
#define MUSIC_LYRIC_STORAGE_META_H_

#include <optional>
#include <string>
#include <vector>

#include "storage/meta/meta.pb.h"

namespace music_lyric_model::storage {
	/**
	 * Creates a Meta, the lyric metadata container.
	 */
	lyric::storage::Meta makeMeta(const lyric::storage::Meta& meta = {});

	/**
	 * Creates a MetaText, a value optionally tagged with a language.
	 */
	lyric::storage::MetaText makeMetaText(const lyric::storage::MetaText& text = {});

	/**
	 * Creates a MetaCredit, a role with its credited names.
	 */
	lyric::storage::MetaCredit makeMetaCredit(const lyric::storage::MetaCredit& credit = {});

	/**
	 * Creates a MetaUnknown, an unrecognized meta preserved by its original key.
	 */
	lyric::storage::MetaUnknown makeMetaUnknown(const lyric::storage::MetaUnknown& unknown = {});

	/**
	 * Creates a MetaReference, a platform and its identifiers.
	 */
	lyric::storage::MetaReference makeMetaReference(const lyric::storage::MetaReference& reference = {});

	/**
	 * Text of a localized meta list, preferring a language match, null when empty.
	 */
	std::optional<std::string> getMetaText(const google::protobuf::RepeatedPtrField<lyric::storage::MetaText>& items, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Unrecognized meta values carrying the given original key.
	 */
	std::vector<std::string> getMetaUnknown(const google::protobuf::RepeatedPtrField<lyric::storage::MetaUnknown>& unknowns, const std::string& key);

	/**
	 * Reference ids for the given platform.
	 */
	std::vector<std::string> getMetaReference(const google::protobuf::RepeatedPtrField<lyric::storage::MetaReference>& references, const std::string& platform);
} // namespace music_lyric_model::storage

#endif
