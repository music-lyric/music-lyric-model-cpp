#include "storage/line.h"

#include "common/line.h"
#include "common/time.h"
#include "common/word.h"

namespace music_lyric_model::storage {
	namespace {
		/**
		 * Translation content from an annotation, preferring a language match.
		 */
		std::optional<std::string> translationOf(const common::LineAnnotation* annotation, const std::optional<std::string>& language) {
			if (!annotation) {
				return std::nullopt;
			}
			const common::LineAnnotationTranslation* item = common::getFirstAnnotation(annotation->translations, language);
			return item ? std::optional<std::string>(item->content) : std::nullopt;
		}

		/**
		 * Roman content from an annotation, preferring a language match.
		 */
		std::optional<std::string> romanOf(const common::LineAnnotation* annotation, const std::optional<std::string>& language) {
			if (!annotation) {
				return std::nullopt;
			}
			const common::LineAnnotationRoman* item = common::getFirstAnnotation(annotation->romans, language);
			return item ? std::optional<std::string>(item->content) : std::nullopt;
		}
	} // namespace

	const common::Time* getStorageLineTime(const Line& line) {
		return line.time.has_value() ? &*line.time : nullptr;
	}

	const common::Time* getStorageLineTime(const LineBackground& line) {
		return line.time.has_value() ? &*line.time : nullptr;
	}

	int64_t getStorageLineDuration(const Line& line) {
		return common::getTimeDuration(getStorageLineTime(line));
	}

	int64_t getStorageLineDuration(const LineBackground& line) {
		return common::getTimeDuration(getStorageLineTime(line));
	}

	const std::vector<common::Word>& getStorageLineWords(const Line& line) {
		return line.words;
	}

	const std::vector<common::Word>& getStorageLineWords(const LineBackground& line) {
		return line.words;
	}

	std::string getStorageLineText(const Line& line) {
		return common::getWordsText(getStorageLineWords(line));
	}

	std::string getStorageLineText(const LineBackground& line) {
		return common::getWordsText(getStorageLineWords(line));
	}

	std::vector<std::string> getStorageLineLanguages(const Line& line) {
		return common::getWordsLanguages(getStorageLineWords(line));
	}

	std::vector<std::string> getStorageLineLanguages(const LineBackground& line) {
		return common::getWordsLanguages(getStorageLineWords(line));
	}

	const common::LineAnnotation* getStorageLineAnnotation(const Line& line) {
		return line.annotation.has_value() ? &*line.annotation : nullptr;
	}

	const common::LineAnnotation* getStorageLineAnnotation(const LineBackground& line) {
		return line.annotation.has_value() ? &*line.annotation : nullptr;
	}

	int getStorageActiveLineIndex(const std::vector<Line>& lines, int64_t ms) {
		for (int i = 0, len = static_cast<int>(lines.size()); i < len; i++) {
			if (common::isTimeActive(getStorageLineTime(lines[static_cast<size_t>(i)]), ms)) {
				return i;
			}
		}
		return -1;
	}

	const Line* getStorageActiveLine(const std::vector<Line>& lines, int64_t ms) {
		const int index = getStorageActiveLineIndex(lines, ms);
		return index == -1 ? nullptr : &lines[static_cast<size_t>(index)];
	}

	std::optional<std::string> getStorageLineTranslation(const Line& line, const std::optional<std::string>& language) {
		return translationOf(getStorageLineAnnotation(line), language);
	}

	std::optional<std::string> getStorageLineTranslation(const LineBackground& line, const std::optional<std::string>& language) {
		return translationOf(getStorageLineAnnotation(line), language);
	}

	std::optional<std::string> getStorageLineRoman(const Line& line, const std::optional<std::string>& language) {
		return romanOf(getStorageLineAnnotation(line), language);
	}

	std::optional<std::string> getStorageLineRoman(const LineBackground& line, const std::optional<std::string>& language) {
		return romanOf(getStorageLineAnnotation(line), language);
	}
} // namespace music_lyric_model::storage
