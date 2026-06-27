#ifndef MUSIC_LYRIC_WORD_ANNOTATION_H_
#define MUSIC_LYRIC_WORD_ANNOTATION_H_

#include "word/annotation.pb.h"

namespace music_lyric_model {
	using namespace lyric;

	/**
	 * Creates a WordAnnotationContent, one token of a word annotation.
	 */
	lyric::WordAnnotationContent makeWordAnnotationContent();

	/**
	 * Creates a WordAnnotationRoman.
	 */
	lyric::WordAnnotationRoman makeWordAnnotationRoman();

	/**
	 * Creates a WordAnnotationRuby.
	 */
	lyric::WordAnnotationRuby makeWordAnnotationRuby();

	/**
	 * Creates a WordAnnotationUnknown.
	 */
	lyric::WordAnnotationUnknown makeWordAnnotationUnknown();

	/**
	 * Creates a WordAnnotation, the per-word annotation container.
	 */
	lyric::WordAnnotation makeWordAnnotation();
}

#endif
