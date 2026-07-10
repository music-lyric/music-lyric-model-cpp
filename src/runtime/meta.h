#ifndef MUSIC_LYRIC_RUNTIME_META_H_
#define MUSIC_LYRIC_RUNTIME_META_H_

#include <optional>
#include <string>
#include <vector>

#include "runtime/meta/meta.pb.h"

namespace music_lyric_model::runtime {
	using namespace lyric::runtime;

	/**
	 * Creates a Meta, the lyric metadata container.
	 */
	lyric::runtime::Meta makeMeta(const lyric::runtime::Meta& meta = {});

	/**
	 * Creates a MetaText, a value optionally tagged with a language.
	 */
	lyric::runtime::MetaText makeMetaText(const lyric::runtime::MetaText& text = {});

	/**
	 * Creates a MetaCredit, a role with its credited names.
	 */
	lyric::runtime::MetaCredit makeMetaCredit(const lyric::runtime::MetaCredit& credit = {});

	/**
	 * Creates a MetaUnknown, an unrecognized meta preserved by its original key.
	 */
	lyric::runtime::MetaUnknown makeMetaUnknown(const lyric::runtime::MetaUnknown& unknown = {});

	/**
	 * Creates a MetaReference, a platform and its identifiers.
	 */
	lyric::runtime::MetaReference makeMetaReference(const lyric::runtime::MetaReference& reference = {});

	/**
	 * Text of a localized meta list, preferring a language match, null when empty.
	 */
	std::optional<std::string> getMetaText(const google::protobuf::RepeatedPtrField<lyric::runtime::MetaText>& items, const std::optional<std::string>& language = std::nullopt);

	/**
	 * Unrecognized meta values carrying the given original key.
	 */
	std::vector<std::string> getMetaUnknown(const google::protobuf::RepeatedPtrField<lyric::runtime::MetaUnknown>& unknowns, const std::string& key);

	/**
	 * Reference ids for the given platform.
	 */
	std::vector<std::string> getMetaReference(const google::protobuf::RepeatedPtrField<lyric::runtime::MetaReference>& references, const std::string& platform);
} // namespace music_lyric_model::runtime

#endif
