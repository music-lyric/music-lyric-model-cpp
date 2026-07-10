#ifndef MUSIC_LYRIC_STORAGE_PART_H_
#define MUSIC_LYRIC_STORAGE_PART_H_

#include "storage/part/part.pb.h"

namespace music_lyric_model::storage {
	/**
	 * Creates a Part, a line's structural section.
	 */
	lyric::storage::Part makePart(const lyric::storage::Part& part = {});
} // namespace music_lyric_model::storage

#endif
