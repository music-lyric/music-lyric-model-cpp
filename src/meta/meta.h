#ifndef MUSIC_LYRIC_META_META_H_
#define MUSIC_LYRIC_META_META_H_

#include <optional>
#include <string>
#include <vector>

#include "meta/meta.pb.h"

namespace music_lyric_model {
	using namespace lyric;

	/**
	 * Creates a Meta, the lyric metadata container.
	 */
	lyric::Meta makeMeta();

	/**
	 * Creates a MetaText, a value optionally tagged with a language.
	 */
	lyric::MetaText makeMetaText();

	/**
	 * Creates a MetaCredit, a role with its credited names.
	 */
	lyric::MetaCredit makeMetaCredit();

	/**
	 * Creates a MetaUnknown, an unrecognized meta preserved by its original key.
	 */
	lyric::MetaUnknown makeMetaUnknown();

	/**
	 * Creates a MetaReference, a platform and its identifiers.
	 */
	lyric::MetaReference makeMetaReference();

	/**
	 * Text of a localized meta list, preferring a language match, null when empty.
	 */
	std::optional<std::string> getMetaText(const google::protobuf::RepeatedPtrField<lyric::MetaText>& items, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Unrecognized meta values carrying the given original key.
	 */
	std::vector<std::string> getMetaUnknown(const google::protobuf::RepeatedPtrField<lyric::MetaUnknown>& unknowns, const std::string& key);

	/**
	 * Reference ids for the given platform.
	 */
	std::vector<std::string> getMetaReference(const google::protobuf::RepeatedPtrField<lyric::MetaReference>& references, const std::string& platform);
} // namespace music_lyric_model

#endif
