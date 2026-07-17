#include "storage/line.h"

#include "common/line.h"
#include "common/time.h"
#include "common/word.h"

namespace music_lyric_model::storage {
	namespace {
		/**
		 * Translation content from an annotation, preferring a language match.
		 */
		std::optional<std::string> translationOf(const lyric::common::LineAnnotation* annotation, const std::optional<std::string>& language) {
			if (!annotation) {
				return std::nullopt;
			}
			const lyric::common::LineAnnotationTranslation* item = common::getFirstAnnotation(annotation->translations(), language);
			return item ? std::optional<std::string>(item->content()) : std::nullopt;
		}

		/**
		 * Roman content from an annotation, preferring a language match.
		 */
		std::optional<std::string> romanOf(const lyric::common::LineAnnotation* annotation, const std::optional<std::string>& language) {
			if (!annotation) {
				return std::nullopt;
			}
			const lyric::common::LineAnnotationRoman* item = common::getFirstAnnotation(annotation->romans(), language);
			return item ? std::optional<std::string>(item->content()) : std::nullopt;
		}
	} // namespace

	lyric::storage::Line makeStorageLine(const lyric::storage::Line& line) {
		return line;
	}

	lyric::storage::LineBackground makeStorageLineBackground(const lyric::storage::LineBackground& background) {
		return background;
	}

	const lyric::common::Time* getStorageLineTime(const lyric::storage::Line& line) {
		return line.has_time() ? &line.time() : nullptr;
	}

	const lyric::common::Time* getStorageLineTime(const lyric::storage::LineBackground& line) {
		return line.has_time() ? &line.time() : nullptr;
	}

	int64_t getStorageLineDuration(const lyric::storage::Line& line) {
		return common::getTimeDuration(getStorageLineTime(line));
	}

	int64_t getStorageLineDuration(const lyric::storage::LineBackground& line) {
		return common::getTimeDuration(getStorageLineTime(line));
	}

	const google::protobuf::RepeatedPtrField<lyric::common::Word>& getStorageLineWords(const lyric::storage::Line& line) {
		return line.words();
	}

	const google::protobuf::RepeatedPtrField<lyric::common::Word>& getStorageLineWords(const lyric::storage::LineBackground& line) {
		return line.words();
	}

	std::string getStorageLineText(const lyric::storage::Line& line) {
		return common::getWordsText(getStorageLineWords(line));
	}

	std::string getStorageLineText(const lyric::storage::LineBackground& line) {
		return common::getWordsText(getStorageLineWords(line));
	}

	std::vector<std::string> getStorageLineLanguages(const lyric::storage::Line& line) {
		return common::getWordsLanguages(getStorageLineWords(line));
	}

	std::vector<std::string> getStorageLineLanguages(const lyric::storage::LineBackground& line) {
		return common::getWordsLanguages(getStorageLineWords(line));
	}

	const lyric::common::LineAnnotation* getStorageLineAnnotation(const lyric::storage::Line& line) {
		return line.has_annotation() ? &line.annotation() : nullptr;
	}

	const lyric::common::LineAnnotation* getStorageLineAnnotation(const lyric::storage::LineBackground& line) {
		return line.has_annotation() ? &line.annotation() : nullptr;
	}

	int getStorageActiveLineIndex(const google::protobuf::RepeatedPtrField<lyric::storage::Line>& lines, int64_t ms) {
		for (int i = 0, len = lines.size(); i < len; i++) {
			if (common::isTimeActive(getStorageLineTime(lines.Get(i)), ms)) {
				return i;
			}
		}
		return -1;
	}

	const lyric::storage::Line* getStorageActiveLine(const google::protobuf::RepeatedPtrField<lyric::storage::Line>& lines, int64_t ms) {
		const int index = getStorageActiveLineIndex(lines, ms);
		return index == -1 ? nullptr : &lines.Get(index);
	}

	std::optional<std::string> getStorageLineTranslation(const lyric::storage::Line& line, const std::optional<std::string>& language) {
		return translationOf(getStorageLineAnnotation(line), language);
	}

	std::optional<std::string> getStorageLineTranslation(const lyric::storage::LineBackground& line, const std::optional<std::string>& language) {
		return translationOf(getStorageLineAnnotation(line), language);
	}

	std::optional<std::string> getStorageLineRoman(const lyric::storage::Line& line, const std::optional<std::string>& language) {
		return romanOf(getStorageLineAnnotation(line), language);
	}

	std::optional<std::string> getStorageLineRoman(const lyric::storage::LineBackground& line, const std::optional<std::string>& language) {
		return romanOf(getStorageLineAnnotation(line), language);
	}
} // namespace music_lyric_model::storage
