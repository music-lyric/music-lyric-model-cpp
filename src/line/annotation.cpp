#include "line/annotation.h"

#include "line/content.h"

namespace music_lyric_model {
	lyric::LineAnnotationUnknown makeLineAnnotationUnknown() {
		return {};
	}

	lyric::LineAnnotationRoman makeLineAnnotationRoman() {
		return {};
	}

	lyric::LineAnnotationTranslate makeLineAnnotationTranslate() {
		return {};
	}

	lyric::LineAnnotation makeLineAnnotation() {
		return {};
	}

	std::optional<std::string> getLineTranslate(const lyric::Line &line, const std::optional<std::string> &language) {
		const lyric::LineAnnotation *annotation = getLineAnnotation(line);
		if (!annotation) {
			return std::nullopt;
		}
		const lyric::LineAnnotationTranslate *item = getFirstAnnotation(annotation->translates(), language);
		return item ? std::optional<std::string>(item->content()) : std::nullopt;
	}

	std::optional<std::string> getLineRoman(const lyric::Line &line, const std::optional<std::string> &language) {
		const lyric::LineAnnotation *annotation = getLineAnnotation(line);
		if (!annotation) {
			return std::nullopt;
		}
		const lyric::LineAnnotationRoman *item = getFirstAnnotation(annotation->romans(), language);
		return item ? std::optional<std::string>(item->content()) : std::nullopt;
	}
}
