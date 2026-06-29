#ifndef MUSIC_LYRIC_META_META_H_
#define MUSIC_LYRIC_META_META_H_

#include <string>
#include <vector>

#include "meta/meta.pb.h"

namespace music_lyric_model {
	using namespace lyric;

	/**
	 * Creates a MetaCreator.
	 */
	lyric::MetaCreator makeMetaCreator();

	/**
	 * Creates a MetaItem; set its typed value through the content oneof.
	 */
	lyric::MetaItem makeMetaItem();

	/**
	 * Whether any meta of a case exists.
	 */
	bool hasMeta(const google::protobuf::RepeatedPtrField<lyric::MetaItem>& metas, lyric::MetaItem::ContentCase kind);

	/**
	 * All metas of a case.
	 */
	std::vector<const lyric::MetaItem*> getAllMeta(const google::protobuf::RepeatedPtrField<lyric::MetaItem>& metas, lyric::MetaItem::ContentCase kind);

	/**
	 * First meta of a case, null when absent.
	 */
	const lyric::MetaItem* getFirstMeta(const google::protobuf::RepeatedPtrField<lyric::MetaItem>& metas, lyric::MetaItem::ContentCase kind);

	/**
	 * All metas with the given original key.
	 */
	std::vector<const lyric::MetaItem*> getMetaByKey(const google::protobuf::RepeatedPtrField<lyric::MetaItem>& metas, const std::string& key);
} // namespace music_lyric_model

#endif
