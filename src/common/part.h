#ifndef MUSIC_LYRIC_MODEL_COMMON_PART_H_
#define MUSIC_LYRIC_MODEL_COMMON_PART_H_

#include <optional>
#include <string>

namespace music_lyric_model::common {
	/**
	 * Structural type of a lyric part.
	 */
	enum class PartType {
		Unspecified  = 0,
		Other        = 1,
		Intro        = 2,
		Verse        = 3,
		PreChorus    = 4,
		Chorus       = 5,
		PostChorus   = 6,
		Bridge       = 7,
		Refrain      = 8,
		Instrumental = 9,
		Outro        = 10,
	};

	/**
	 * A line's structural section.
	 */
	struct Part {
		PartType                 type  = PartType::Unspecified;
		std::optional<std::string> label;
	};
} // namespace music_lyric_model::common

#endif
