#ifndef MUSIC_LYRIC_LINE_ANNOTATION_H_
#define MUSIC_LYRIC_LINE_ANNOTATION_H_

#include "line/annotation.pb.h"

namespace music_lyric_model {
	using namespace lyric;

	/**
	 * Creates a LineAnnotationUnknown.
	 */
	lyric::LineAnnotationUnknown makeLineAnnotationUnknown();

	/**
	 * Creates a LineAnnotationRoman.
	 */
	lyric::LineAnnotationRoman makeLineAnnotationRoman();

	/**
	 * Creates a LineAnnotationTranslate.
	 */
	lyric::LineAnnotationTranslate makeLineAnnotationTranslate();

	/**
	 * Creates a LineAnnotation, the per-line annotation container.
	 */
	lyric::LineAnnotation makeLineAnnotation();
}

#endif
