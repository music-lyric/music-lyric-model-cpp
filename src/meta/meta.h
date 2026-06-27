#ifndef MUSIC_LYRIC_META_META_H_
#define MUSIC_LYRIC_META_META_H_

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
}

#endif
